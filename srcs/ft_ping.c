#include "../includes/ping.h"

static void help() {
  printf("ft_ping help");
  printf("\n");
}

static void freePing(t_ping *ping, int pack) {
  if (pack == 1)
    free(ping->packet);
  free(ping);
}

static unsigned short calculate_checksum(unsigned short *paddress, int len) {
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

static int parse(t_ping *ping, int ac, char **av) {
  int opt = 0;
  while ((opt = getopt(ac, av, "hv")) != -1) {
    switch (opt) {
    case 'h':
      ping->help();
      break;
    case 'v':
      ping->verose = 1;
      break;
    }
  }
  if (optind < ac) {
    ping->hostname = av[optind];
  } else {
    fprintf(stderr, "Usage: %s, [-h] [-v] ip_adress\n", av[0]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static int ft_send(t_ping *ping) {
  ping->header(ping);
  if (sendto(ping->sockfd, ping->packet, ping->pacetSize, 0,
             (struct sockaddr *)&ping->dest_addr,
             sizeof(ping->dest_addr)) < 0) {
    perror("Error sendto");
    return EXIT_FAILURE;
  }
  ping->seq++;
  return EXIT_SUCCESS;
}

static int ft_receive(t_ping *ping) {
  int ret = 0;
  char buf[1024];
  struct sockaddr_in from;
  socklen_t fromlen = sizeof(from);

  ret = recvfrom(ping->sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&from,
                 &fromlen);
  if (ret < 0) {
    return EXIT_FAILURE;
  }

  printf("%d bytes from %s: seq=%d ttl=%d time=0.%.1ld ms\n", 
            ret, inet_ntoa(from.sin_addr), ping->seq, ping->ttl, ping->tv.tv_usec / 1000);
  ping->seqRecv++;
  return EXIT_SUCCESS;
}

static void fill_seq_icmp(t_ping *ping){
  ping->icmp_header->icmp_type = ICMP_ECHO;
  ping->icmp_header->icmp_code = 0;
  ping->icmp_header->icmp_cksum = 0;
  ping->icmp_header->icmp_id = getpid() & 0xFFFF;
  ping->icmp_header->icmp_seq = htons(ping->seq);
  ping->icmp_header->icmp_cksum =
      calculate_checksum((unsigned short *)ping->icmp_header, ping->pacetSize);
}

static int openSocket(t_ping *ping) {
    // need check packet size default at 56
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
  if (ping->getname(ping)){
      return EXIT_FAILURE;
  }
  ping->icmp_header = (struct icmp *)ping->packet;
  ping->dest_addr.sin_family = AF_INET;
  ping->dest_addr.sin_addr.s_addr = inet_addr(ping->ip);
  return EXIT_SUCCESS;
}

static void closePing(t_ping *ping){
    printf("--- %s ping statistics ---\n", ping->hostname);
    printf("%d packets transmitted, %d packets receive, %f.2%% packet loss\n", ping->seq, ping->seqRecv, 0.0);
    ping->free(ping, 1);
    close(ping->sockfd);
}

static int host_to_ip(t_ping *ping){
    char host[100];
    struct addrinfo hint;
    struct addrinfo *servinfo, *tmp;
    bzero(&hint, sizeof(hint));
    hint.ai_family = AF_INET;
    
    int recv = getaddrinfo(ping->hostname, NULL, &hint, &servinfo);
    if (recv < 0){
        perror("Error getaddrinfo");
        return EXIT_FAILURE;
    }
    for (tmp = servinfo; tmp != NULL; tmp = tmp->ai_next){
        getnameinfo(tmp->ai_addr, tmp->ai_addrlen, host, sizeof(host), NULL, 0, NI_NUMERICHOST);
        strcpy(ping->ip, host);
    }
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
  ping->close = &closePing;
  ping->getname = &host_to_ip;
  ping->header = fill_seq_icmp;
  ping->ttl = 64;
  return (ping);
}
