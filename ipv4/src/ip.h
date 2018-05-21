//
// Created by Maciej Ziółkowski on 17.05.2018.
//

#ifndef PROJEKT_IP_H
#define PROJEKT_IP_H

struct ip {
    unsigned int ip_v:4;           /* wersja */
    unsigned char ip_hl:4;        /* dlugosc naglowka*/
    unsigned char ip_tos;            /* rodzaj uslugi */
    unsigned short ip_len;            /* calkowita dlugosc*/
    unsigned short ip_id;            /* identyfikator */
    unsigned short ip_off;            /* fragment pola offset*/
    unsigned char ip_ttl;            /* czaszycia*/
    unsigned char ip_p;            /* protokol */
    unsigned int ip_src_addr;
    unsigned int ip_dest_addr;
    unsigned short ip_sum;            /* checksum */
    struct eleBufora *nowyEleIp;
};
#endif //PROJEKT_IP_H
