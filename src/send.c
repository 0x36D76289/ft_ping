#include "../include/ft_ping.h"
#include "../include/utils.h"
#include "../include/send.h"

void send_packet(t_ping *ping)
{
    char packet[PACKET_SIZE];
    struct icmphdr *icmp_hdr;
    char *data;

    memset(packet, 0, PACKET_SIZE);
    icmp_hdr = (struct icmphdr *)packet;
    data = packet + sizeof(struct icmphdr);

    icmp_hdr->type = ICMP_ECHO;
    icmp_hdr->code = 0;
    icmp_hdr->un.echo.id = ping->ident;
    icmp_hdr->un.echo.sequence = ping->seq_num++;
    icmp_hdr->checksum = 0;

    snprintf(data, DATA_SIZE, "ft_ping data seq=%d", icmp_hdr->un.echo.sequence);
    icmp_hdr->checksum = checksum((unsigned short *)packet, PACKET_SIZE);
    gettimeofday(&ping->send_time, NULL);
    ssize_t bytes_sent = sendto(ping->sockfd, packet, PACKET_SIZE, 0,
                                (struct sockaddr *)&ping->dest_addr, sizeof(ping->dest_addr));
    if (bytes_sent < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            printf("Request timeout for icmp_seq %d\n", icmp_hdr->un.echo.sequence);
        else
            perror("ft_ping: sendto");
    }
    else
        ping->packets_sent++;
}
