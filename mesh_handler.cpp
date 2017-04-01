#include "mesh_handler.h"
#include "socket_handler.h"
#include "message_converter.h"

MeshHandler MeshHandler::instance{};

MeshHandler& MeshHandler::getInstance() {
  return instance;
}

void MeshHandler::setupMesh()
{
    mesh.setNodeID(0);
    mesh.begin(108, RF24_250KBPS);

    radio.printDetails();
}

boost::thread MeshHandler::startListening() {
  std::cerr << "MeshHandler listening..." << std::endl;

  return boost::thread(boost::bind(&MeshHandler::loop, this));
}


void MeshHandler::updateMesh()
{
    mesh.update();
    mesh.DHCP();
}

int MeshHandler::readAvailableData(std::vector<device_data>& buffer)
{
  int dataCount = 0;

  while(network.available())
  {
    RF24NetworkHeader header;
    network.peek(header);

    radio_device_data dat;

    switch(header.type)
    {
      case 'S':
      {
        network.read(header,&dat,sizeof(dat));

        device_data tmp {mesh.getNodeID(header.from_node), dat.data, std::string(dat.type), "sensor"};

        printf("Rcv sensor data from ID#%d (addr: %d): type:%s data:%f \n", tmp.id, header.from_node, tmp.type.c_str(), tmp.data);

        buffer.push_back(tmp);

        dataCount++;
      }
        break;
      case 'A':
      {
        network.read(header,&dat,sizeof(dat));

        device_data tmp {mesh.getNodeID(header.from_node), dat.data, std::string(dat.type), "actor"};

        printf("Rcv actor data from ID#%d (addr: %d): type:%s data:%f \n", tmp.id, header.from_node, tmp.type.c_str(), tmp.data);

        buffer.push_back(tmp);

        dataCount++;
      }
        break;
      default:
        network.read(header,0,0);
        printf("Rcv bad type %d from ID %o (addr: %o)\n",header.type,mesh.getNodeID(header.from_node), header.from_node);
        break;
    }

  }
  return dataCount;
}

void MeshHandler::sendToDevice(device_command command) {
  command_buffer_mutex.lock();

  command_buffer.push_back(command);

  command_buffer_mutex.unlock();
}

void MeshHandler::printAddressTable()
{
  printf("\n");
  printf("********Assigned Addresses********\n");
   for(int i=0; i<mesh.addrListTop; i++){
     printf("NodeID: %u RF24Network Address: %d\n", mesh.addrList[i].nodeID, mesh.addrList[i].address);

   }
  printf("**********************************\n");
}

void MeshHandler::loop() {

  boost::posix_time::ptime last = boost::posix_time::second_clock::local_time();

  while (1) {
    updateMesh();

    std::vector<device_data> buffer;

    readAvailableData(buffer);

    for (device_data d : buffer) {
      SocketHandler::getInstance().sendString(MessageConverter::getInstance().convertDeviceDataToJson(d));
    }

    if (command_buffer_mutex.try_lock()) {
      for (device_command command : command_buffer) {
        std::cerr << "Command sending: {id:" << command.id << ", targetState:" << command.targetState << "} ... ";
        radio_device_command tmp{command.targetState};

        if (mesh.write(&tmp, 'A', sizeof(tmp), (uint8_t) command.id)) {
          std::cerr << "success" << std::endl;
        } else {
          std::cerr << "FAIL" << std::endl;
        }
      }

      command_buffer.clear();
      command_buffer_mutex.unlock();
    }

    boost::posix_time::time_duration diff = boost::posix_time::second_clock::local_time() - last;
    if (diff.total_seconds() >= 20) {
      printAddressTable();

      last = boost::posix_time::second_clock::local_time();
    }

    boost::this_thread::sleep_for(boost::chrono::milliseconds{2});
  }
}
