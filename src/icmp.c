#include "../inc/ft_ping.h"

unsigned short calculate_checksum(unsigned short *buffer, int length)
{
    unsigned long sum = 0;
    for (; length > 1; length -= 2)
        sum += *buffer++;
    if (length == 1)
        sum += *(unsigned char *)buffer;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

void create_icmp_packet(t_ping *ping, char *packet)
{
    struct icmphdr *icmp_header = (struct icmphdr *)packet;

    icmp_header->type = ICMP_ECHO;
    icmp_header->code = 0;
    icmp_header->un.echo.id = htons(ping->identifiant);
    icmp_header->un.echo.sequence = htons(ping->sequence++);
    icmp_header->checksum = 0;

    memset(packet + sizeof(struct icmphdr), 'a', ping->data_size);

    icmp_header->checksum = calculate_checksum((unsigned short *)packet, sizeof(struct icmphdr) + ping->data_size);
}

int unpack_icmp_packet(char *buffer, int size, t_ping *ping, struct timeval recv_time)
{
    struct iphdr *ip_header = (struct iphdr *)buffer;
    struct icmphdr *icmp_header = (struct icmphdr *)(buffer + (ip_header->ihl * 4));

    if (icmp_header->type == ICMP_ECHOREPLY && icmp_header->un.echo.id == htons(ping->identifiant))
    {
        long double rtt = (recv_time.tv_sec - ping->time_start.tv_sec) * 1000.0 + (recv_time.tv_usec - ping->time_start.tv_usec) / 1000.0;
        int seq = ntohs(icmp_header->un.echo.sequence);
        rtt -= (seq) * 1000;
        gettimeofday(&ping->time_end, NULL);
        printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.2Lf ms\n", size - (int)(ip_header->ihl * 4), inet_ntoa(ping->dest_addr.sin_addr), seq, ip_header->ttl, rtt);
        return 1;
    }
    else if (ping->options.verbose && icmp_header->type != ICMP_ECHO)
        printf("Received ICMP packet with type %d\n", icmp_header->type);
    return 0;
}
