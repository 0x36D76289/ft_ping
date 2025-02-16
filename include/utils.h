#ifndef UTILS_H
#define UTILS_H

#include <netinet/in.h> // For htons

unsigned short checksum(unsigned short *addr, int len);

#endif