//
// Created by maciej on 08.05.18.
//


#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>           // strcpy, memset(), and memcpy()
#include <netinet/in.h>       // IPPROTO_RAW, IPPROTO_UDP, INET6_ADDRSTRLEN
#include <netinet/ip6.h>      // struct ip6_hdr


unsigned char * CreateIPV4Packet() {
    unsigned char *datagram = malloc(4096);    //rezerwujemy pamiec dla pakietu
    struct iphdr *iph = (struct iphdr *) datagram;

}