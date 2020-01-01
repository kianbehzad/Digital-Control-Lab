#include "digital.h"

std::string serialBytes(double vel_F, double vel_N, double vel_w, int id)
{
    static unsigned char visionCounter = 0;
    const int _ROBOT_PACKET_SIZE {14};
    const int _MAX_ROBOT_NUM {12};
    unsigned char robotPackets[_MAX_ROBOT_NUM][_ROBOT_PACKET_SIZE] {};
    unsigned char outputBuffer[_ROBOT_PACKET_SIZE] = {};
    auto robotId = static_cast<unsigned char>(id);
    int kickNumber = static_cast<int>(round(0.));

    outputBuffer[0] = 0x99;
    outputBuffer[1] = robotId & static_cast<unsigned char>(0x0F);
    /* if (_packet->roller_speed != 0) {
         outputBuffer[1] = static_cast<unsigned char>(outputBuffer[1] | ((_packet->roller_speed & 0x07) << 4));
     }*/
    outputBuffer[2] = static_cast<unsigned char>(kickNumber & 0x7F);
    outputBuffer[3] = static_cast<unsigned char>((kickNumber >> 7) & 0x07);

    int vforwardI = static_cast<int>(floor(vel_F * 487.0));
    int vnormalI  = static_cast<int>(floor(vel_N * 487.0));
    int vangularI  = static_cast<int>((vel_w) * ((double)256.0 / (double) 360.0));

    if (vforwardI > 2047) {
        vforwardI = 2047;
    }
    if (vforwardI < -2047) {
        vforwardI = -2047;
    }

    if (vnormalI > 2047) {
        vnormalI = 2047;
    }
    if (vnormalI < -2047) {
        vnormalI = -2047;
    }

    if (vangularI > 2047) {
        vangularI = 2047;
    }
    if (vangularI < -2047) {
        vangularI = -2047;
    }

    int vangularAbs = abs(vangularI);
    int vforwardAbs = abs(vforwardI);
    int vnormalAbs  = abs(vnormalI);

    outputBuffer[3] = static_cast<unsigned char>((((vangularAbs >> 7) & 0x0F) << 3) | outputBuffer[3]);

    outputBuffer[4] = static_cast<unsigned char>(vforwardAbs & 0x7F);
    outputBuffer[5] = static_cast<unsigned char>(vnormalAbs & 0x7F);
    outputBuffer[6] = static_cast<unsigned char>((vforwardAbs >> 7) & 0x0F);
    if (vforwardI < 0) {
        outputBuffer[6] = static_cast<unsigned char>(outputBuffer[6] | 0x10);
    }
    if (vnormalI < 0) {
        outputBuffer[6] = static_cast<unsigned char>(outputBuffer[6] | 0x20);
    }

    outputBuffer[7] = static_cast<unsigned char>(vangularAbs & 0x7F);


    outputBuffer[8] |= 0x01;


    //if (_packet->chip) {
    //    outputBuffer[8] = outputBuffer[8] | 0x02;
    // }
    if (vangularI < 0) {
        outputBuffer[8] = outputBuffer[8] | 0x04;
    }
    outputBuffer[8] = outputBuffer[8] | (((vnormalAbs >> 7) & 0x0F) << 3);


    outputBuffer[11] =  visionCounter & 0x07;
    outputBuffer[12] = 0x3F & 0x00;
    outputBuffer[13] = 0;

    for (int k{} ; k < _ROBOT_PACKET_SIZE ; k++)
    {
        robotPackets[robotId][k] = outputBuffer[k];
    }

    for (int myid{}; myid < _MAX_ROBOT_NUM; myid++)
    {
        for (int k{} ; k < _ROBOT_PACKET_SIZE ; k++)
        {
            if(myid != id)
            {
                unsigned char outputBuffer[_ROBOT_PACKET_SIZE] = {};
                auto robotId = static_cast<unsigned char>(15);
                outputBuffer[0] = 0x99;
                outputBuffer[1] = robotId & static_cast<unsigned char>(0x0F);
                robotPackets[myid][k] = outputBuffer[k];
            }
        }
    }
    if (visionCounter < 8) {
        visionCounter ++;
    } else {
        visionCounter = 0;
    }

//    for(int i=0; i<_MAX_ROBOT_NUM; i++)
//    {
//        for (int k{}; k < _ROBOT_PACKET_SIZE; k++)
//        {
//            std::bitset<8> x(robotPackets[i][k]);
//            std::cout << x << "    ";
//        }
//        std::cout << std::endl;
//    }

    std::string s{};
    for(int i=0; i<_ROBOT_PACKET_SIZE; i++)
    {
        s.push_back(robotPackets[robotId][i]);
    }
    const std::string res{s};


    return res;
}
