//
// Created by Maciej Ziółkowski on 17.05.2018.
//

#ifndef PROJEKT_IP_H
#define PROJEKT_IP_H

struct ip {
    __u8 ihl:4,
            version:4;
    __u8 tos;
    __be16 tot_len;
    __be16 id;
    __be16 frag_off;
    __u8 ttl;
    __u8 protocol;
    __sum16 check;
    __be32 saddr;
    __be32 daddr;

};
#endif //PROJEKT_IP_H
