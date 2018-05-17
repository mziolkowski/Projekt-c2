//
// Created by Maciej Ziółkowski on 17.05.2018.
//

#ifndef PROJEKT_ICMP_H
#define PROJEKT_ICMP_H

#include <opencl-c.h>

struct icmphdr {
    uint8 type;
    uint8 code;
    uint16 checksum;
    struct eleBufora *nowyEle;
};

#endif //PROJEKT_ICMP_H
