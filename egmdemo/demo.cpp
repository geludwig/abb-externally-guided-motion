/*
geludwig 2024
https://github.com/geludwig
*/

/*
This is a simple example on how to use the ABB EGM (Externally Guided Motion) interface.
It includes all necessary logic to open a EGM udp interface and execute a movement in cartesic x-axis direction.

The codebase is written for linux! To make it work on Windows, the socket functions and the GetTickCount function have to be ported.

Non-standard convention:
The value 0 is defined as false.
All values != 0 are false and used as error codes.
*/

#include "demo.h"

int main(int argc, char* argv[])
{
    std::cout << "[EGM] thread started." << std::endl;

    // variables for demo
    int temp = 0;
	int egmInitSuccess = 0;
    int egmTimeout = 30; // seconds to wait for initial egm message from robot

    // init egm objects
    EgmPositionsFeedback egmFeedback;
    EgmPositionsTarget egmTarget;

    // init egm objects with values
    egmTarget.x = 1000;
    egmTarget.y = 0;
    egmTarget.z = 1000;
    egmTarget.q0 = 0;
    egmTarget.q1 = 0;
    egmTarget.q2= 1;
    egmTarget.q3= 0;
    egmTarget.xSpeed = 0;
    egmTarget.ySpeed = 0;
    egmTarget.zSpeed = 0;

    // init udp socket
    UdpSocket socket0;
    socket0.port = 6510; // default port for ABB EGM is 6510
    if (socket0.openSock() != 0)
    {
        std::cout << "[EGM] socket open error" << std::endl;
        return 1;
    }

    // wait for initial robot egm interface message
    temp = 0;
    std::cout << "[EGM] waiting for robot interface." << std::endl;
    while (temp < egmTimeout)
    {
		// timeout of 1sec is set by UdpSocket::openSock()
        if (receiveRobotMessage(socket0, egmFeedback) == 0)
        {
            std::cout << "[EGM] robot interface open." << std::endl;
            egmInitSuccess = 1;
            break;
        }
        if (temp > egmTimeout)
        {
            std::cout << "[EGM] robot interface timeout." << std::endl;
            break;
        }
        temp++;
    }

    // main work loop
    while (egmInitSuccess >= 1)
    {
        // receive egm positions to check if connection is still alive and to remain in sync with robot controller (important!)
        if (receiveRobotMessage(socket0, egmFeedback) != 0)
        {
            std::cout << "[EGM] robot interface down." << std::endl;
            egmInitSuccess = 0;
            break;
        }
        else
        {
			// print feedback position
			std::cout << "[EGM] yFeedback: " << egmFeedback.x << std::endl;
			
            // stop condition when x position (1500mm) is reached by robot
            if (egmFeedback.x >= 1500)
            {
                egmInitSuccess = 0;
                break;
            }

            // manipulate position (always send cartesic position + corresponding speed value)
            egmTarget.x = egmTarget.x + 0.1;    // new position = old position + offset
            egmTarget.xSpeed = 250 * 0.1;       // speed = 250Hz * offset

            // send position
            sendRobotMessage(socket0, egmTarget);
        }
    }

    // close udp socket
    if (socket0.closeSock() < 0)
    {
        std::cout << "[EGM] socket close error" << std::endl;
    }

    return 0;
}