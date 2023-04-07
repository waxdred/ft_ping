#ifndef FT_PING
#define FT_PING
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
#include <sys/_types/_timeval.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_ping {
  t_stat stat;

  char *hostname;
  char *packet;
  char ip[100];

  int alloc;
  int pacetSize;
  int seqRecv;
  int sockfd;
  int ttl;
  int verose;
  uint16_t seq;

  struct icmp *icmp_header;
  struct sockaddr_in dest_addr;
  struct timeval start;
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
