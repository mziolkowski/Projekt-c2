//
// Created by Maciej Ziółkowski on 17.05.2018.
//

#ifndef PROJEKT_ICMP_HDR_H
#define PROJEKT_ICMP_HDR_H

#include <linux/types.h>

struct icmp_hdr {
    __u8 type;
    __u8 code;
    __sum16 checksum;
    union {
        struct {
            __be16 id;
            __be16 sequence;
        } echo;
        __be32 gateway;
        struct {
            __be16 __unused;
            __be16 mtu;
        } frag;
    } un;

};
#endif //PROJEKT_ICMP_HDR_H

