#include "../includes/ping.h"

int signalStop;

static void handle_signal() { signalStop = 0; }
static void help(char *s) {
  fprintf(stderr, "Usage: %s, [-h] [-v] ip_adress\n", s);
}

static void freePing(t_ping *ping) {
  if (ping->alloc == 1)
    free(ping->packet);
  free(ping);
}

static unsigned short calculate_checksum(void *addr, size_t count) {
  unsigned short *ptr;
  unsigned long sum;

  ptr = addr;
  for (sum = 0; count > 1; count -= 2)
    sum += *ptr++;
  if (count > 0)
    sum += *(unsigned char *)ptr;
  while (sum >> 16)
    sum = (sum & 0xffff) + (sum >> 16);
  return (~sum);
}

static int parse(t_ping *ping, int ac, char **av) {
  int opt = 0;
  while ((opt = getopt(ac, av, "hv")) != -1) {
    switch (opt) {
    case 'h':
      ping->help(av[0]);
      break;
    case 'v':
      ping->verose = 1;
      break;
    }
  }
  if (optind < ac) {
    ping->hostname = av[optind];
  } else {
    fprintf(stderr, "ft_ping: Usage: %s, [-h] [-v] ip_adress\n", av[0]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static int ft_send(t_ping *ping) {
  ping->header(ping);
  if (sendto(ping->sockfd, ping->packet, ping->packetSize, 0,
             (struct sockaddr *)&ping->dest_addr,
             sizeof(ping->dest_addr)) < 0) {
    fprintf(stderr, "ft_ping: error sending packet: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }
  ping->seq++;
  return EXIT_SUCCESS;
}

static int ft_receive(t_ping *ping, struct timeval dev) {
  int ret = 0;
  char buf[1024];
  struct timeval end;
  struct sockaddr_in from;
  socklen_t fromlen = sizeof(from);

  bzero(buf, 1024);
  ret = recvfrom(ping->sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&from,
                 &fromlen);
  if (ret < 0) {
    return EXIT_FAILURE;
  }
  gettimeofday(&end, NULL);
  double data = (double)(end.tv_usec - ping->tv.tv_usec) / 1000;
  double stddev = (double)(end.tv_usec - dev.tv_usec) / 1000;
  struct ip *ip = (struct ip *)buf;
  printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3lf ms\n",
         ip->ip_len, ping->hostname, ping->ip, ping->seq, ip->ip_ttl, data);
  ping->seqRecv++;
  ping->stat.insert(&ping->stat, data, DATA);
  ping->stat.insert(&ping->stat, stddev, DEV);
  return EXIT_SUCCESS;
}

static void fill_seq_icmp(t_ping *ping) {
  ping->icmp_header->icmp_type = ICMP_ECHO;
  ping->icmp_header->icmp_code = 0;
  ping->icmp_header->icmp_id = getpid() & 0xFFFF;
  ping->icmp_header->icmp_seq = htons(ping->seq);
  memset(ping->icmp_header->icmp_data, '*', ping->packetSize);
  ping->icmp_header->icmp_cksum = 0;
  ping->icmp_header->icmp_cksum =
      calculate_checksum((unsigned short *)ping->icmp_header, ping->packetSize);
}

static int openSocket(t_ping *ping) {
  struct timeval timeout = {1, 0};
  socklen_t len = sizeof(timeout);
  ping->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (ping->sockfd == -1) {
    fprintf(stderr, "ft_ping: error creation socket: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }
  ping->packetSize = sizeof(struct icmp) + sizeof(struct timeval);
  ping->packet = (char *)malloc(ping->packetSize);
  if (!ping->packet) {
    fprintf(stderr, "ft_ping: error allocation: %s\n", strerror(errno));
    return 2;
  }
  memset(ping->packet, 0, sizeof(struct icmp));
  ping->alloc = 1;
  if (ping->getname(ping)) {
    return EXIT_FAILURE;
  }
  // set time out for receive
  if (setsockopt(ping->sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, len) < 0) {
    fprintf(stderr, "ft_ping: error set setsockopt timeout: %s\n",
            strerror(errno));
    return EXIT_FAILURE;
  }
  // set ttl
  if (setsockopt(ping->sockfd, IPPROTO_IP, IP_TTL, &ping->ttl,
                 sizeof(ping->ttl)) == -1) {
    fprintf(stderr, "ft_ping: error set setsockopt ttl: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }
  ping->dest_addr.sin_family = AF_INET;
  ping->dest_addr.sin_addr.s_addr = inet_addr(ping->ip);
  ping->icmp_header = (struct icmp *)ping->packet;
  ping->icmp_header->icmp_id = getpid() & 0xFFFF;
  ping->header(ping);
  return EXIT_SUCCESS;
}

static double getPourcente(t_ping *ping) {
  if (ping->seqRecv == 0)
    return 100;
  double ret = ping->seq - ping->seqRecv;
  return ((ret / ping->seq) * 100 / 100);
}

static void closePing(t_ping *ping) {
  struct timeval end;
  gettimeofday(&end, NULL);
  printf("--- %s ping statistics ---\n", ping->hostname);
  printf(
      "%d packets transmitted, %d packets receive, %.2lf%% packet loss, time "
      "%.3lfms\n",
      ping->seq, ping->seqRecv, getPourcente(ping),
      (double)(end.tv_usec - ping->start.tv_usec) / 1000);
  printf("round-trip min/avg/max/stddev = %.2lf/%.2lf/%.2lf/%.2lf\n",
         ping->stat.min(&ping->stat, DATA), ping->stat.avg(&ping->stat, DATA),
         ping->stat.max(&ping->stat, DATA), ping->stat.avg(&ping->stat, DEV));
  ping->stat.free(&ping->stat);
  ping->free(ping);
  close(ping->sockfd);
}

static int host_to_ip(t_ping *ping) {
  char host[100];
  struct addrinfo hint;
  struct addrinfo *servinfo, *tmp;
  bzero(&hint, sizeof(hint));
  hint.ai_family = AF_INET;

  int recv = getaddrinfo(ping->hostname, NULL, &hint, &servinfo);
  if (recv < 0) {
    perror("ft_ping: error getaddrinfo");
    return EXIT_FAILURE;
  } else if (servinfo == NULL) {
    fprintf(stderr, "ft_ping: cannot resolve: %s: Unknown host\n",
            ping->hostname);
    return EXIT_FAILURE;
  }
  for (tmp = servinfo; tmp != NULL; tmp = tmp->ai_next) {
    getnameinfo(tmp->ai_addr, tmp->ai_addrlen, host, sizeof(host), NULL, 0,
                NI_NUMERICHOST);
    strcpy(ping->ip, host);
  }
  return EXIT_SUCCESS;
}

int run_ping(t_ping *ping) {
  struct timeval dev;
  signalStop = 1;
  signal(SIGINT, handle_signal);

  printf("PING %s (%s): %d data bytes\n", ping->hostname, ping->ip,
         ping->packetSize);
  gettimeofday(&ping->start, NULL);
  while (signalStop) {
    sleep(1);
    gettimeofday(&ping->tv, NULL);
    if (ping->send(ping)) {
      perror("ft_ping: error sendto");
      ping->close(ping);
      break;
    }
    gettimeofday(&dev, NULL);
    ping->receive(ping, dev);
  }
  ping->close(ping);
  return EXIT_SUCCESS;
}

t_ping *initPing() {
  t_ping *ping = malloc(sizeof(t_ping));

  if (ping == NULL)
    return NULL;
  bzero(ping, sizeof(t_ping));
  initStat(&ping->stat);
  ping->help = &help;
  ping->free = &freePing;
  ping->parse = &parse;
  ping->connection = &openSocket;
  ping->send = &ft_send;
  ping->receive = &ft_receive;
  ping->close = &closePing;
  ping->getname = &host_to_ip;
  ping->run = &run_ping;
  ping->header = fill_seq_icmp;
  ping->ttl = 64;
  return (ping);
}
