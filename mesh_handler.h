#ifndef MESH_HANDLER_H
#define MESH_HANDLER_H

#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include "RF24Mesh/RF24Mesh.h"
#include <iostream>
#include <string>

struct sensor_data {
    short id;
    float data;
    std::string type;
};

class MeshHandler {
public:
    void setupMesh();
    
    void updateMesh();
    
    int readAvailableData(std::vector<sensor_data>& buffer);
    
    void printAddressTable();
    
    
    MeshHandler():radio(22, 0), network(radio), mesh(radio, network) {
        
    }
    
private:
    RF24 radio;
    RF24Network network;
    RF24Mesh mesh;
    
};

#endif
