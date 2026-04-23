#include "../include/ft_ping.h"
#include "../include/receive.h"

void receive_packet(t_ping *ping)
{
    char buffer[PACKET_SIZE + sizeof(struct ip)];
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);
    struct timeval recv_time;
    ssize_t bytes_received;

    bytes_received = recvfrom(ping->sockfd, buffer, sizeof(buffer), 0,
                              (struct sockaddr *)&sender_addr, &sender_addr_len);

    gettimeofday(&recv_time, NULL);

    if (bytes_received < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return; 
        else
        {
            perror("ft_ping: recvfrom");
            return;
        }
    }

    struct ip *ip_hdr = (struct ip *)buffer;
    struct icmphdr *icmp_hdr = (struct icmphdr *)(buffer + (ip_hdr->ip_hl << 2));

    char sender_ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(sender_addr.sin_addr), sender_ip_str, INET_ADDRSTRLEN);

    if (icmp_hdr->type == ICMP_ECHOREPLY)
    {
        if (icmp_hdr->un.echo.id == ping->ident)
        {
            ping->packets_received++;
            double rtt = (recv_time.tv_sec - ping->send_time.tv_sec) * 1000.0 +
                         (recv_time.tv_usec - ping->send_time.tv_usec) / 1000.0;

            printf("%zd bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
                   bytes_received - (ip_hdr->ip_hl << 2), sender_ip_str, icmp_hdr->un.echo.sequence,
                   ip_hdr->ip_ttl, rtt);
        }
    }
    else if (ping->verbose)
    {
        printf("Received ICMP message from %s: type=%d, code=%d\n",
               sender_ip_str, icmp_hdr->type, icmp_hdr->code);
    }
}
