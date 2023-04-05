#include "../includes/ping.h"
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

void help() {
  printf("ft_ping help");
  printf("\n");
}

void freePing(t_ping *ping, int pack) {
  if (pack == 1)
    free(ping->packet);
  free(ping);
}

unsigned short calculate_checksum(unsigned short *paddress, int len) {
  int nleft = len;
  int sum = 0;
  unsigned short *w = paddress;
  unsigned short answer = 0;

  while (nleft > 1) {
    sum += *w++;
    nleft -= 2;
  }

  if (nleft == 1) {
    *(unsigned char *)(&answer) = *(unsigned char *)w;
    sum += answer;
  }

  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  answer = ~sum;
  return answer;
}

int parse(t_ping *ping, int ac, char **av) {
  int opt = 0;
  while ((opt = getopt(ac, av, "hv")) != -1) {
    switch (opt) {
    case 'h':
      ping->help();
    case 'v':
      ping->verose = 1;
    }
  }
  if (optind < ac) {
    ping->adress = av[optind];
  } else {
    fprintf(stderr, "Usage: %s, [-h] [-v] ip_adress\n", av[0]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int ft_send(t_ping *ping) {
  if (sendto(ping->sockfd, ping->packet, ping->pacetSize, 0,
             (struct sockaddr *)&ping->dest_addr,
             sizeof(ping->dest_addr)) < 0) {
    perror("Error sendto");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int ft_receive(t_ping *ping) {
  int ret = 0;
  char buf[1024];
  struct sockaddr_in from;
  socklen_t fromlen = sizeof(from);

  ret = recvfrom(ping->sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&from,
                 &fromlen);
  if (ret < 0) {
    return EXIT_FAILURE;
  }
  ping->pacetRecv++;
  printf("Received %d bytes from %s\n", ret, inet_ntoa(from.sin_addr));
  return EXIT_SUCCESS;
}

int openSocket(t_ping *ping) {
  ping->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (ping->sockfd == -1) {
    perror("Error socket");
    return EXIT_FAILURE;
  }
  ping->pacetSize = sizeof(struct icmp) + sizeof(struct timeval);
  ping->packet = (char *)malloc(ping->pacetSize);
  if (!ping->packet) {
    perror("Error malloc");
    return 2;
  }
  ping->icmp_header = (struct icmp *)ping->packet;
  ping->icmp_header->icmp_type = ICMP_ECHO;
  ping->icmp_header->icmp_code = 0;
  ping->icmp_header->icmp_cksum = 0;
  ping->icmp_header->icmp_id = getpid() & 0xFFFF;
  ping->icmp_header->icmp_seq = 0;
  ping->icmp_header->icmp_cksum =
      calculate_checksum((unsigned short *)ping->icmp_header, ping->pacetSize);
  ping->dest_addr.sin_family = AF_INET;
  ping->dest_addr.sin_addr.s_addr = inet_addr(ping->adress);
  return EXIT_SUCCESS;
}

t_ping *initPing() {
  t_ping *ping = malloc(sizeof(t_ping));

  if (ping == NULL)
    return NULL;
  bzero(ping, sizeof(t_ping));
  ping->help = &help;
  ping->free = &freePing;
  ping->parse = &parse;
  ping->connection = &openSocket;
  ping->send = &ft_send;
  ping->receive = &ft_receive;
  return (ping);
}
