#ifndef FT_PING_H
#define FT_PING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <signal.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024
#define DEFAULT_DATA_SIZE 56
#define DEFAULT_TTL 64

typedef struct s_options
{
    bool verbose;
    bool help;
} t_options;

typedef struct s_ping
{
    int sockfd;
    struct sockaddr_in dest_addr;
    char *hostname;
    int id;
    int seq;
    int ttl;
    int data_size;
    t_options options;
    int num_packets_sent;
    int num_packets_received;
    struct timeval time_start;
    struct timeval time_end;
} t_ping;

// icmp.c
unsigned short calculate_checksum(unsigned short *buffer, int length);
void create_icmp_packet(t_ping *ping, char *packet);
int unpack_icmp_packet(char *buffer, int size, t_ping *ping, struct timeval recv_time);

// utils.c
void set_addrinfo(t_ping *ping);

// options.c
void parse_arguments(int argc, char **argv, t_ping *ping);
void print_usage(void);

// error.c
void error_exit(const char *msg);

// main.c
void signal_handler(int sig);

#endif
