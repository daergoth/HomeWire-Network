#include "mesh_handler.h"


void MeshHandler::setupMesh()
{
    mesh.setNodeID(0);
    mesh.begin(108, RF24_250KBPS);
    
    radio.printDetails();
}

void MeshHandler::updateMesh()
{
    mesh.update();
    mesh.DHCP();
}

int MeshHandler::readAvailableData(std::vector<sensor_data>& buffer)
{
  struct inner_sensor_data {
    float data;
    char type[15];
  };
    
  int dataCount = 0;
    
  while(network.available())
  {
    RF24NetworkHeader header;
    network.peek(header);
        
    inner_sensor_data dat;
        
    switch(header.type)
    {
      case 'S': 
      {
        network.read(header,&dat,sizeof(dat)); 
            
        sensor_data tmp {mesh.getNodeID(header.from_node), dat.data, std::string(dat.type)};
            
        printf("Rcv sensor data from ID#%d (addr: %d): type:%s data:%f \n", tmp.id, header.from_node, tmp.type.c_str(), tmp.data);
            
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


void MeshHandler::printAddressTable()
{
    printf("\n");
    printf("********Assigned Addresses********\n");
     for(int i=0; i<mesh.addrListTop; i++){
       printf("NodeID: %u RF24Network Address: %d\n", mesh.addrList[i].nodeID, mesh.addrList[i].address);
     }
    printf("**********************************\n");
}

