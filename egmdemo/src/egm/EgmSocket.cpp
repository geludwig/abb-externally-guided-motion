/*
geludwig 2024
https://github.com/geludwig
*/

#include "EgmSocket.h"

// Open UDP socket
int UdpSocket::openSock()
{
    sockaddr_in serverAddr, clientAddr;
    socklen = sizeof(clientAddr);
    sockfd = ::socket(AF_INET, SOCK_DGRAM, 0);

    // Init socket
    if (sockfd < 0)
    {
        std::cout << "Socket initalization failed." << std::endl;
        return 1;
    }

    // Set socket options
    struct timeval tv;
    tv.tv_sec = 1;		// timeout in seconds
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv)) < 0)
    {
        std::cout << "Socket set timeout failed." << std::endl;
        return 1;
    }

    // Set socket definitions
    std::memset(&serverAddr, sizeof(serverAddr), 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    // Bind socket
    int sockerr = ::bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    if (sockerr != 0)
    {
        std::cout << "Socket bind failed." << std::endl;
        return 1;
    }

    return 0;
}

// Close UDP socket
int UdpSocket::closeSock()
{
    int sockerr = close(sockfd);

    if (sockerr != 0)
    {
        std::cout << "Socket close failed." << std::endl;
        return 1;
    }

    return 0;
}

// Receive UDP Protobuf message
int UdpSocket::receiveSock(char* messageBuffer)
{
    int sockerr = recvfrom(sockfd, messageBuffer, 1400, 0, (struct sockaddr*)&clientAddr, &socklen);

    return sockerr;
}

// Send UDP Protobuf message
int UdpSocket::sendSock(std::string messageBuffer)
{
    int sockerr = sendto(sockfd, messageBuffer.c_str(), messageBuffer.length(), 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));

    return sockerr;
}
