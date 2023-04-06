#ifndef FT_PING
#define FT_PING value
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>

#define BUFF_SIZE 1024
typedef struct s_ping {
  char *hostname;
  char ip[100];
  char *packet;
  int (*connection)(struct s_ping *);
  int (*parse)(struct s_ping *, int ac, char **av);
  int (*receive)(struct s_ping *);
  int (*send)(struct s_ping *);
  int pacetSize;
  int seq;
  int seqRecv;
  int sockfd;
  int ttl;
  int verose;
  struct icmp *icmp_header;
  struct sockaddr_in dest_addr;
  struct timeval tv;
  void (*close)(struct s_ping *);
  void (*free)(struct s_ping *, int pack);
  void (*header)(struct s_ping *);
  void (*help)(void);
  int (*getname)(struct s_ping*);

} t_ping;
t_ping *initPing();
#endif /* ifndef  FT_PING */
