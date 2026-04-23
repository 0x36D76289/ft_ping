#include "../include/ft_ping.h"
#include "../include/parse.h"

void parse_args(int argc, char **argv, t_ping *ping)
{
    int opt;

    ping->verbose = false;
    ping->ttl = 64;
    ping->seq_num = 0;
    ping->packets_sent = 0;
    ping->packets_received = 0;

    while ((opt = getopt(argc, argv, "v?")) != -1)
    {
        switch (opt)
        {
        case 'v':
            ping->verbose = true;
            break;
        case '?':
            printf("Usage: %s [-v] [-?] <hostname or IPv4 address>\n", argv[0]);
            printf("  -v: Verbose mode (display detailed ICMP error information)\n");
            printf("  -?: Show this help message\n");
            exit(EXIT_SUCCESS);
        default:
            fprintf(stderr, "Usage: %s [-v] [-?] <hostname or IPv4 address>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    if (optind >= argc)
    {
        fprintf(stderr, "ft_ping: missing host operand\n");
        exit(EXIT_FAILURE);
    }
    ping->hostname = argv[optind];
}

void resolve_hostname(t_ping *ping)
{
    struct addrinfo hints, *result, *rp;
    int s;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;    // IPv4
    hints.ai_socktype = SOCK_RAW; // ICMP
    hints.ai_protocol = IPPROTO_ICMP;

    s = getaddrinfo(ping->hostname, NULL, &hints, &result);
    if (s != 0)
    {
        fprintf(stderr, "ft_ping: %s: %s\n", ping->hostname, gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        if (rp->ai_family == AF_INET)
        {
            ping->dest_addr = *(struct sockaddr_in *)rp->ai_addr;
            freeaddrinfo(result);
            return;
        }
    }
    fprintf(stderr, "ft_ping: Could not resolve hostname: %s\n", ping->hostname);
    freeaddrinfo(result);
    exit(EXIT_FAILURE);
}
