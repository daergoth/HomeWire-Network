#ifndef MESH_HANDLER_H
#define MESH_HANDLER_H

#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include "RF24Mesh/RF24Mesh.h"
#include <iostream>
#include <string>
#include <vector>
#include <boost/thread.hpp>

struct sensor_data {
    short id;
    float data;
    std::string type;
};

struct actor_command {
    short id;
    bool targetState;
};

class MeshHandler {
public:
    static MeshHandler& getInstance();

    void setupMesh();

    boost::thread startListening();
    
    void updateMesh();
    
    int readAvailableData(std::vector<sensor_data>& buffer);

    bool writeToActor(actor_command command);
    
    void printAddressTable();
    
private:
    static MeshHandler instance;

    RF24 radio;
    RF24Network network;
    RF24Mesh mesh;

    MeshHandler():radio(22, 0), network(radio), mesh(radio, network) {}

    void loop();

    struct radio_sensor_data {
        float data;
        char type[15];
    };

    struct radio_actor_command {
        bool targetState;
    };
    
};

#endif
