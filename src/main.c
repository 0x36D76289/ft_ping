#include "../include/ft_ping.h"
#include "../include/parse.h"
#include "../include/send.h"
#include "../include/receive.h"

t_ping g_ping; // Global to be accessible by signal handler

int main(int argc, char **argv)
{

    parse_args(argc, argv, &g_ping);
    resolve_hostname(&g_ping);
    create_socket(&g_ping);

    g_ping.ident = getpid();
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(g_ping.dest_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
    printf("PING %s (%s) %d(%ld) bytes of data.\n", g_ping.hostname, ip_str, (int)DATA_SIZE, sizeof(struct ip) + PACKET_SIZE);
    gettimeofday(&g_ping.start_time, NULL);
    signal(SIGINT, handle_signal);
    signal(SIGALRM, handle_signal);
    alarm(1); // first alarm

    ft_ping_loop();
    return 0;
}