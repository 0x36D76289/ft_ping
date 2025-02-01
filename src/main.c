#include "../inc/ft_ping.h"

t_ping g_ping;

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        printf("\n--- %s ping statistics ---\n", g_ping.hostname);
        printf("%d packets transmitted, %d received, %.2f%% packet loss, time %.0fms\n", g_ping.num_packets_sent, g_ping.num_packets_received, (g_ping.num_packets_sent - g_ping.num_packets_received) * 100.0 / g_ping.num_packets_sent, (g_ping.time_end.tv_sec - g_ping.time_start.tv_sec) * 1000.0 + (g_ping.time_end.tv_usec - g_ping.time_start.tv_usec) / 1000.0);
        close(g_ping.sockfd);
        exit(0);
    }
    else if (sig == SIGALRM)
    {
        char packet[BUFFER_SIZE];
        create_icmp_packet(&g_ping, packet);
        sendto(g_ping.sockfd, packet, sizeof(struct icmphdr) + g_ping.data_size, 0, (struct sockaddr *)&g_ping.dest_addr, sizeof(g_ping.dest_addr));
        g_ping.num_packets_sent++;
        alarm(1);
    }
}

int main(int argc, char **argv)
{
    g_ping.identifiant = getpid();
    g_ping.sequence = 0;
    g_ping.ttl = DEFAULT_TTL;
    g_ping.data_size = DEFAULT_DATA_SIZE;
    g_ping.options.verbose = false;
    g_ping.options.help = false;
    g_ping.num_packets_sent = 0;
    g_ping.num_packets_received = 0;

    parse_arguments(argc, argv, &g_ping);

    if (g_ping.options.help)
    {
        print_usage();
        return 0;
    }

    if (g_ping.hostname == NULL)
    {
        print_usage();
        error_exit("ft_ping: missing host operand");
    }

    set_addrinfo(&g_ping);

    if ((g_ping.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
        error_exit("ft_ping: failed to create socket");

    setsockopt(g_ping.sockfd, IPPROTO_IP, IP_TTL, &g_ping.ttl, sizeof(g_ping.ttl));

    signal(SIGINT, signal_handler);
    signal(SIGALRM, signal_handler);

    printf("PING %s (%s) %d(%lu) bytes of data.\n", g_ping.hostname, inet_ntoa(g_ping.dest_addr.sin_addr), g_ping.data_size, g_ping.data_size + sizeof(struct icmphdr));

    gettimeofday(&g_ping.time_start, NULL);
    alarm(1);

    char recv_buffer[BUFFER_SIZE];
    struct timeval recv_time;
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);

    while (1)
    {
        int bytes_received = recvfrom(g_ping.sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *)&sender, &sender_len);
        gettimeofday(&recv_time, NULL);

        if (bytes_received > 0 && (unpack_icmp_packet(recv_buffer, bytes_received, &g_ping, recv_time)))
            g_ping.num_packets_received++;
    }

    return 0;
}
