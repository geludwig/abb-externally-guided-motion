/*
geludwig 2024
https://github.com/geludwig
*/

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "egm.pb.h"


struct UdpSocket
{
        unsigned int port;
        unsigned int sockfd;
        socklen_t socklen;
        struct sockaddr_in clientAddr;

        int openSock();
        int closeSock();
        int sendSock(std::string);
        int receiveSock(char*);
};