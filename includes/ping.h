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
#include <unistd.h>

#define BUFF_SIZE 1024
typedef struct s_ping {
  int sockfd;
  int pacetSize;
  int verose;
  int pacetRecv;
  struct sockaddr_in dest_addr;
  struct icmp *icmp_header;
  char *packet;
  char *adress;
  void (*help)(void);
  void (*free)(struct s_ping *, int pack);
  int (*connection)(struct s_ping *);
  int (*send)(struct s_ping *);
  int (*receive)(struct s_ping *);
  int (*parse)(struct s_ping *, int ac, char **av);

} t_ping;
t_ping *initPing();
void freePing(t_ping *ping, int pack);
#endif /* ifndef  FT_PING */
