///* send icmp packet example */
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
//#include <linux/ip.h>
//#include <linux/icmp.h>
#include "ipv4/src/icmp_hdr.h"
#include "ipv4/src/ip.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
unsigned short in_cksum(unsigned short *addr, int len) {
    register int sum = 0;
    u_short answer = 0;
    register u_short *w = addr;
    register int nleft = len;
    /*
     * Algorytm uzywa 32 bitowego akumulatora(sumy), dodajemy do niego
     * sekwencyjne 16-bitowe słowa, a na koniec składamy wszystkie bity
     * przenoszące z 16 pierwszych bitów do 16 ostatnich bitów.
     */
    while (nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }
//  zgaś nieparzysty bajt, jeśli to konieczne
    if (nleft == 1) {
        *(u_char *) (&answer) = *(u_char *) w;
        sum += answer;
    }
    /* dodajemy z powrotem od 16 do 16 bitow */
    sum = (sum >> 16) + (sum & 0xffff);     /* dodajemy pierwsze 16 z starszymi 16 bitami*/
    sum += (sum >> 16);
    answer = ~sum;              /* obciecie do 16 bitow */
    return (answer);
}


int main(int argc, char *argv[]) {
    struct ip *ip;
    struct icmp_hdr *icmp;
    struct sockaddr_in connection;
    char dst_addr[32];
    char *src_addr = "127.0.0.1";
    char *packet, *buffer;
    int sockfd, optval, addrlen;
    int i = 0;
    int liczbaPakietow, ihl, version, protocol1, type, code, sequence, id;
    packet = malloc(sizeof(struct ip) + sizeof(struct icmp_hdr));
    buffer = malloc(sizeof(struct ip) + sizeof(struct icmp_hdr));
    ip = (struct ip *) packet;
    icmp = (struct icmp_hdr *) (packet + sizeof(struct ip));

    printf("\nIle wyslac pakietow? ");
    scanf("%d", &liczbaPakietow);
     printf("\nPodaj adres ip docelowy ");
    scanf("%s", dst_addr);
//    printf("%s", dst_addr);
    printf("\nPodaj dlugosc nagłówka ip (domyslnie 5) ");
    scanf("%d", &ihl);
//    printf("\nPodaj wersje protokolu (domyslnie 4) ");
//    scanf("%d", &version);
//    printf("\nPodaj numer protokolu (domyslnie ICMP - 1) ");
//    scanf("%d", &protocol1);
    printf("\nPodaj typ protokolu ICMP (domyslnie ICMP_ECHO - 8) ");
    scanf("%d", &type);
//    printf("\nPodaj kod ICMP (domyslnie 0) ");
//    scanf("%d", &code);
//    printf("\nPodaj numer sekwencji ICMP (od 0 do 100, domyslnie 100) ");
//    scanf("%d", &sequence);
//    printf("\nPodaj id ICMP (od 0 do 100, domyslnie 55)  ");
//    scanf("%d", &id);
    ip->ihl = ihl;
    ip->version = 4;
    ip->tot_len = sizeof(struct ip) + sizeof(struct icmp_hdr);
    ip->protocol = IPPROTO_ICMP; //ICMP
    ip->saddr = inet_addr(src_addr);
    ip->daddr = inet_addr(dst_addr);
    ip->check = in_cksum((unsigned short *) ip, sizeof(struct ip));
    icmp->type = type; //ICMP_ECHO
    icmp->code = 0;
    icmp->un.echo.sequence = (__be16) 100;
    icmp->un.echo.id = (__be16) 55;
    //checksum
    icmp->checksum = in_cksum((unsigned short *) icmp, sizeof(struct icmp_hdr));
    /* open ICMP socket */
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    /* IP_HDRINCL musi być ustawiony na gnieździe, aby kernel
     * nie próbował automatycznie dodać domyślnego nagłówka ip do pakietu*/
    setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(int));
    connection.sin_family = AF_INET;
    connection.sin_addr.s_addr = ip->daddr;
    while (i < liczbaPakietow) {
        sendto(sockfd, packet, ip->tot_len, 0, (struct sockaddr *) &connection, sizeof(struct sockaddr));
        printf("Sent %d byte packet to %s\n", ip->tot_len, dst_addr);
        i++;
    }
}

