#include "Network.h"
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <iostream>
#include <fcntl.h>

Network::Network() {
    isServer = false;
    connected = false;
    sockfd = -1;
    clientfd = -1;
}

Network::~Network() {
    closeConnection();
}

bool Network::initServer() {
    isServer = true;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return false;

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) return false;

    listen(sockfd, 1);

    std::cout << "Waiting for client..." << std::endl;
    socklen_t clilen = sizeof(cli_addr);
    clientfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if (clientfd < 0) return false;

    connected = true;
    std::cout << "Client connected!" << std::endl;
    return true;
}

bool Network::initClient(const std::string& ip) {
    isServer = false;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return false;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) return false;

    std::cout << "Connecting to " << ip << "..." << std::endl;
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) return false;

    connected = true;
    std::cout << "Connected to server!" << std::endl;
    return true;
}

void Network::sendTankState(TankState state) {
    if (!connected) return;
    int fd = isServer ? clientfd : sockfd;
    send(fd, &state, sizeof(state), 0);
}

bool Network::receiveTankState(TankState& state) {
    if (!connected) return false;
    int fd = isServer ? clientfd : sockfd;

    int n = recv(fd, &state, sizeof(state), MSG_DONTWAIT);
    if (n == sizeof(state)) return true;
    return false;
}

void Network::closeConnection() {
    if (clientfd != -1) close(clientfd);
    if (sockfd != -1) close(sockfd);
    connected = false;
}
