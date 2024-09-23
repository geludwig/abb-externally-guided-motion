/*
Georg Ludwig 2024
https://github.com/geludwig
*/

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "egm.pb.h"
#include "EgmPositions.h"
#include "EgmSocket.h"
#pragma comment(lib, "libprotobuf.a")


int receiveRobotMessage(UdpSocket &socket, EgmPositionsFeedback &posdata);

int sendRobotMessage(UdpSocket &socket, EgmPositionsTarget posdata);