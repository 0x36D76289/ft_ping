#include "../inc/ft_ping.h"

void error_exit(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}
