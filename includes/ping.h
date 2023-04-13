#ifndef FT_PING
#define FT_PING
#include "../libft/include/libft.h"
#include "./stat.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#if defined(__APPLE__)
#define __OS__ 0
#else
#define __OS__ 1
#endif

typedef struct s_check {
  int8_t ok;
  int value;
} t_check;

typedef struct s_flag {
  t_check verbose;
  t_check timeout;
  t_check count;
  t_check ttl;
  t_check silence;
  t_check runtime;

} t_flag;

typedef struct s_ping {
  t_stat stat;
  t_flag flag;

  char *hostname;
  char *packet;
  char ip[100];

  int alloc;
  int packetSize;
  int seqRecv;
  int sockfd;
  int ttl;

  int cont;
  uint16_t seq;

  struct timeval timeout;

  struct icmp *icmp_header;
  struct sockaddr_in dest_addr;
  struct timeval start;
  struct timeval runtime;
  struct timeval tv;

  void (*close)(struct s_ping *ping);
  void (*free)(struct s_ping *ping);
  void (*header)(struct s_ping *ping);
  void (*help)(char *arg);

  int (*connection)(struct s_ping *ping);
  int (*getname)(struct s_ping *ping);
  int (*parse)(struct s_ping *ping, int ac, char **av);
  int (*receive)(struct s_ping *ping, struct timeval);
  int (*run)(struct s_ping *ping);
  int (*send)(struct s_ping *ping);
} t_ping;
t_ping *initPing();
#endif
