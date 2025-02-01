#include "../inc/ft_ping.h"

void set_addrinfo(t_ping *ping)
{
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;

    int status = getaddrinfo(ping->hostname, NULL, &hints, &result);
    if (status != 0)
    {
        fprintf(stderr, "ft_ping: %s: %s\n", ping->hostname, gai_strerror(status));
        exit(2);
    }

    ping->dest_addr = *(struct sockaddr_in *)result->ai_addr;
    freeaddrinfo(result);
}
