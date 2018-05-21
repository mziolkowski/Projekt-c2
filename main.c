///* send icmp packet example */
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <linux/icmp.h>
#include <arpa/inet.h>
#include "ipv4/src/ip.h"
#include "ipv4/src/icmp_hdr.h"

unsigned short in_cksum(unsigned short *addr, int len) {
    register int sum = 0;
    u_short answer = 0;
    register u_short *w = addr;
    register int nleft = len;
    /*
     * Our algorithm is simple, using a 32 bit accumulator (sum), we add
     * sequential 16 bit words to it, and at the end, fold back all the
     * carry bits from the top 16 bits into the lower 16 bits.
     */
    while (nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }
    /* mop up an odd byte, if necessary */
    if (nleft == 1) {
        *(u_char *) (&answer) = *(u_char *) w;
        sum += answer;
    }
    /* add back carry outs from top 16 bits to low 16 bits */
    sum = (sum >> 16) + (sum & 0xffff);     /* add hi 16 to low 16 */
    sum += (sum >> 16);             /* add carry */
    answer = ~sum;              /* truncate to 16 bits */
    return (answer);
}


int main(void) {
    struct ip *pierwszyElementIp = NULL;
    struct ip *poprzedniEleIp = NULL, *biezacyEleIp, *ip_reply;
    struct icmp_hdr *pierwszyElementICMP = NULL;
    struct icmp_hdr *poprzedniEleIcmp = NULL, *biezacyEleICMP;

    struct sockaddr_in connection;
    char *dst_addr = "192.168.0.1";
    char *src_addr = "192.168.0.115";
    char *packet, *buffer;
    int sockfd, optval, addrlen;
    int i = 0;
    int idIp, idIcmp, liczbaPakietow, dlugoscNaglowka, numerSekwencji;


    packet = malloc(sizeof(struct ip) + sizeof(struct icmp_hdr));
    buffer = malloc(sizeof(struct ip) + sizeof(struct icmp_hdr));
    biezacyEleIp = (struct ip *) packet;
    biezacyEleICMP = (struct icmp_hdr *) (packet + sizeof(struct ip));

    printf("\nIle wyslac pakietow? ");
    scanf("%d", &liczbaPakietow);

    while (i < liczbaPakietow) {
        // Przydzielenie bloku o rozmiarze struktury. Biezacy element to wsk. na poczatek blok
        biezacyEleIp = (struct ip *) malloc(
                sizeof(struct ip));
        if (pierwszyElementIp == NULL)
            pierwszyElementIp = biezacyEleIp;
        else {
            poprzedniEleIp->nowyEleIp = biezacyEleIp;
        }

//    printf("\nPodaj dlugosc naglowka: ");
//    scanf("%d", &dlugoscNaglowka);
//    printf("\nPodaj id: ");
//    scanf("%d", &idIp);
        biezacyEleIp->nowyEleIp = NULL;
        biezacyEleIp->ip_v = 4;
        biezacyEleIp->ip_hl = 5;
        biezacyEleIp->ip_len = sizeof(struct ip) + sizeof(struct icmp_hdr);
//    biezacyEleIp->ip_id = (unsigned short) idIp;
        biezacyEleIp->ip_p = IPPROTO_ICMP;
        biezacyEleIp->ip_src_addr = inet_addr(src_addr);
        biezacyEleIp->ip_dest_addr = inet_addr(dst_addr);
        biezacyEleIp->ip_sum = in_cksum((unsigned short *) biezacyEleIp, sizeof(struct icmp_hdr));

        biezacyEleICMP = (struct icmp_hdr *) malloc(
                sizeof(struct icmp_hdr));
        if (pierwszyElementICMP == NULL)
            pierwszyElementICMP = biezacyEleICMP;
        else
            poprzedniEleIcmp->nowyEleICMP = biezacyEleICMP;

        biezacyEleICMP->nowyEleICMP = NULL;
        biezacyEleICMP->type = ICMP_ECHO;
        biezacyEleICMP->code = 0;
//    printf("\nPodaj numer sekwencji: ");
//    scanf("%d", &numerSekwencji);

        biezacyEleICMP->un.echo.sequence = (__be16) rand();
//    printf("\nPodaj id: ");
//    scanf("%d", &idIcmp);

        biezacyEleICMP->un.echo.id = (__be16) rand();
        biezacyEleICMP->icmp_sum = in_cksum((unsigned short *) biezacyEleICMP, sizeof(struct icmp_hdr));
        poprzedniEleIp = biezacyEleIp;
        poprzedniEleIcmp = biezacyEleICMP;
        i++;
    }

    /* open ICMP socket */
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    /* IP_HDRINCL must be set on the socket so that the kernel does not attempt
    *  to automatically add a default ip header to the packet*/
    setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(int));

    connection.sin_family = AF_INET;
    connection.sin_addr.s_addr = biezacyEleIp->ip_dest_addr;
    biezacyEleIp = pierwszyElementIp;
    biezacyEleICMP = pierwszyElementICMP;

    if(i == liczbaPakietow) {
        while (i > 0 && biezacyEleIp != NULL && biezacyEleICMP != NULL) {
            sendto(sockfd, packet, biezacyEleIp->ip_len, 0, (struct sockaddr *) &connection, sizeof(struct sockaddr));
            printf("Sent %d byte packet to %s\n", biezacyEleIp->ip_len, dst_addr);
            i--;
            biezacyEleIp = biezacyEleIp->nowyEleIp;
            biezacyEleICMP = biezacyEleICMP->nowyEleICMP;
        }
        printf("\n");
        i = 0;

        // Kasownie elementow z listy wiazanej
        while (biezacyEleIp != NULL && biezacyEleICMP != NULL) {
            biezacyEleIp = pierwszyElementIp;
            poprzedniEleIp = biezacyEleIp;
            biezacyEleIp = biezacyEleIp->nowyEleIp;
            free(poprzedniEleIp);

            biezacyEleICMP = pierwszyElementICMP;
            poprzedniEleIcmp = biezacyEleICMP;
            biezacyEleICMP= biezacyEleICMP->nowyEleICMP;
            free(poprzedniEleIcmp);
        }
    }

}

