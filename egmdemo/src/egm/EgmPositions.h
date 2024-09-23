/*
geludwig 2024
https://github.com/geludwig
*/

#pragma once

/* EGM POSITIONS STRUCT */
struct EgmPositionsTarget
{
    double x = 0;
    double y = 0;
    double z = 0;
    double q0 = 0;
    double q1 = 0;
    double q2 = 0;
    double q3 = 0;
    double xSpeed = 0;
    double ySpeed = 0;
    double zSpeed = 0;
    double rot0Speed = 0;
    double rot1Speed = 0;
    double rot2Speed = 0;
};

struct EgmPositionsFeedback
{
    double x = 0;
    double y = 0;
    double z = 0;
    double q0 = 0;
    double q1= 0;
    double q2= 0;
    double q3 = 0;
};