/*
Georg Ludwig 2024
https://github.com/geludwig
*/

#include "EgmProto.h"

// GetTickCount Unix
int GetTick(void)
{
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now))
        return 0;
    return now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0;
}

// Parse receive message from protobuf to pointer
int parseReceiveProtoMessage(abb::egm::EgmRobot* pRobotMessage, EgmPositionsFeedback& egmData)
{
    if (pRobotMessage->has_header())
    {
        egmData.x = pRobotMessage->feedback().cartesian().pos().x();
        egmData.y = pRobotMessage->feedback().cartesian().pos().y();
        egmData.z = pRobotMessage->feedback().cartesian().pos().z();
        egmData.q0 = pRobotMessage->feedback().cartesian().orient().u0();
        egmData.q1 = pRobotMessage->feedback().cartesian().orient().u1();
        egmData.q2 = pRobotMessage->feedback().cartesian().orient().u2();
        egmData.q3 = pRobotMessage->feedback().cartesian().orient().u3();
    }

    else
    {
        std::cout << "Proto header missing." << std::endl;
        return 1;
    }

    return 0;
}

// Parse send message from pointer to protobuf
void parseSendProtoMessage(abb::egm::EgmSensor* pSensorMessage, EgmPositionsTarget& egmData)
{
    static unsigned int seq = 0; // static variables are init only once

    // Header
    abb::egm::EgmHeader* header = new abb::egm::EgmHeader();
    header->set_mtype(abb::egm::EgmHeader_MessageType_MSGTYPE_CORRECTION);
    header->set_seqno(seq++);
    header->set_tm(GetTick());
    pSensorMessage->set_allocated_header(header);

    // Planned Cartesian
    abb::egm::EgmCartesian* pc = new abb::egm::EgmCartesian();
    pc->set_x(egmData.x);
    pc->set_y(egmData.y);
    pc->set_z(egmData.z);

    abb::egm::EgmQuaternion* pq = new abb::egm::EgmQuaternion();
    pq->set_u0(egmData.q0);
    pq->set_u1(egmData.q1);
    pq->set_u2(egmData.q2);
    pq->set_u3(egmData.q3);

    abb::egm::EgmPose* pcartesian = new abb::egm::EgmPose();
    pcartesian->set_allocated_orient(pq);
    pcartesian->set_allocated_pos(pc);

    abb::egm::EgmPlanned* planned = new abb::egm::EgmPlanned();
    planned->set_allocated_cartesian(pcartesian);

    pSensorMessage->set_allocated_planned(planned);

    // Planned Speedref
    abb::egm::EgmCartesianSpeed* pspeed = new abb::egm::EgmCartesianSpeed;
    pspeed->add_value(egmData.xSpeed);
    pspeed->add_value(egmData.ySpeed);
    pspeed->add_value(egmData.zSpeed);
    pspeed->add_value(egmData.rot0Speed);
    pspeed->add_value(egmData.rot1Speed);
    pspeed->add_value(egmData.rot2Speed);

    abb::egm::EgmSpeedRef* pspeedref = new abb::egm::EgmSpeedRef;
    pspeedref->set_allocated_cartesians(pspeed);

    pSensorMessage->set_allocated_speedref(pspeedref);
}

// Receive Udp message
int receiveRobotMessage(UdpSocket &socket, EgmPositionsFeedback &egmData)
{
    char messageBuffer[1400];
    int sockerr = socket.receiveSock(messageBuffer);

    if (sockerr < 0)
    {
        return 1;
    }
    else
    {
        abb::egm::EgmRobot* pRobotMessage = new abb::egm::EgmRobot();
        pRobotMessage->ParseFromArray(messageBuffer, sockerr);
        parseReceiveProtoMessage(pRobotMessage, egmData);
        delete pRobotMessage;
    }

    return 0;
}

// Send Udp message
int sendRobotMessage(UdpSocket &socket, EgmPositionsTarget egmData)
{
    std::string messageBuffer;

    abb::egm::EgmSensor* pSensorMessage = new abb::egm::EgmSensor();
    parseSendProtoMessage(pSensorMessage, egmData);
    pSensorMessage->SerializeToString(&messageBuffer);

    int sockerr = socket.sendSock(messageBuffer);

    if (sockerr < 0) {
        return 1;
    }
    else {
        delete pSensorMessage;
        return 0;
    }
    return 0;
}
