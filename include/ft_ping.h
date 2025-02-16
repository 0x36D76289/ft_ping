#ifndef FT_PING_H
#define FT_PING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>

// Constants
#define PACKET_SIZE 64
#define DATA_SIZE (PACKET_SIZE - sizeof(struct icmphdr))
#define MAX_WAIT_TIME 1 // Timeout in seconds

// Structure to hold program state
typedef struct
{
    int sockfd;
    struct sockaddr_in dest_addr;
    int ident;
    int seq_num;
    int ttl;
    bool verbose;
    int packets_sent;
    int packets_received;
    struct timeval start_time;
    struct timeval send_time;
    char *hostname;
} t_ping;

// Global variable (for signal handler)
extern t_ping g_ping;

// Function prototypes (moved to appropriate headers)
void print_stats(t_ping *ping);
void handle_signal(int sig);
void set_socket_options(t_ping *ping);
void create_socket(t_ping *ping);
void ft_ping_loop();

#endif