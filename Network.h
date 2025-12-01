#ifndef NETWORK_H
#define NETWORK_H

#include "Utils.h"
#include <string>
#include <netinet/in.h>

class Network {
public:
    bool isServer;
    int sockfd;
    int clientfd; // For server to hold client connection
    struct sockaddr_in serv_addr, cli_addr;
    bool connected;

    Network();
    ~Network();
    bool initServer();
    bool initClient(const std::string& ip);
    void sendTankState(TankState state);
    bool receiveTankState(TankState& state);
    void closeConnection();
};

#endif
