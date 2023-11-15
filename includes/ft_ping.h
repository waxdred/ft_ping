#ifndef FT_PING_H
#define FT_PING_H
#include "./ft_stat.h"
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

typedef struct s_recv {
  int ret;
  double data;
  double stddev;
  char buf[1024];
  struct timeval end;
  struct sockaddr_in from;
  socklen_t fromlen;
} t_recv;

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
  int ac;
  int packetSize;
  int seqRecv;
  int sockfd;
  int ttl;

  int cont;
  int8_t signal;
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

char *ft_strchr(const char *str, int c);
char *ft_strcpy(const char *s1, char *dest);
double getPourcente(t_ping *ping);
int ft_atoi(const char *str);
int ft_receive(t_ping *ping, struct timeval dev);
int ft_send(t_ping *ping);
int host_to_ip(t_ping *ping);
int openSocket(t_ping *ping);
int parse(t_ping *ping, int ac, char **av);
int run_ping(t_ping *ping);
int8_t cmptv(struct timeval tv1, struct timeval tv2, int sec);
t_ping *get_ping(t_ping *ping);
t_ping *initPing(int ac);
unsigned short calculate_checksum(void *addr, size_t count);
void *ft_memset(void *b, int c, size_t len);
void closePing(t_ping *ping);
void fill_seq_icmp(t_ping *ping);
void freePing(t_ping *ping);
void ft_bzero(void *s, size_t n);
void handle_signal(int sig);
void handler_alarm(int sig);
void help(char *s);
#endif
