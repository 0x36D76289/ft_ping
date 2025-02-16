#ifndef PARSE_H
#define PARSE_H

#include "ft_ping.h"

void parse_args(int argc, char **argv, t_ping *ping);
void resolve_hostname(t_ping *ping);

#endif