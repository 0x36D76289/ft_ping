#include "../include/ft_ping.h"
#include "../include/utils.h"
#include "../include/send.h"
#include "../include/receive.h"

void create_socket(t_ping *ping)
{
    ping->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (ping->sockfd < 0)
    {
        perror("ft_ping: socket");
        exit(EXIT_FAILURE);
    }
    if (setuid(getuid()) == -1)
    {
        perror("ft_ping: setuid (dropping privileges)");
        exit(EXIT_FAILURE);
    }
    set_socket_options(ping);
}

void set_socket_options(t_ping *ping)
{
    struct timeval timeout;
    timeout.tv_sec = MAX_WAIT_TIME;
    timeout.tv_usec = 0;

    if (setsockopt(ping->sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
    {
        perror("ft_ping: setsockopt (SO_RCVTIMEO)");
        exit(EXIT_FAILURE);
    }
}

void print_stats(t_ping *ping)
{
    struct timeval end_time;
    gettimeofday(&end_time, NULL);

    double total_time = (end_time.tv_sec - ping->start_time.tv_sec) * 1000.0 +
                        (end_time.tv_usec - ping->start_time.tv_usec) / 1000.0;

    printf("\n--- %s ping statistics ---\n", ping->hostname);
    printf("%d packets transmitted, %d received, %.2f%% packet loss, time %.0fms\n",
           ping->packets_sent, ping->packets_received,
           (ping->packets_sent > 0) ? (double)(ping->packets_sent - ping->packets_received) / ping->packets_sent * 100.0 : 0.0,
           total_time);
}

void handle_signal(int sig)
{
    if (sig == SIGINT)
    {
        print_stats(&g_ping);
        close(g_ping.sockfd);
        exit(EXIT_SUCCESS);
    }
    else if (sig == SIGALRM)
    {
        send_packet(&g_ping);
        receive_packet(&g_ping);
        alarm(1);
    }
}

void ft_ping_loop()
{
    while (1)
        pause();
}
