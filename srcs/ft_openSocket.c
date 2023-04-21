#include "../includes/ft_ping.h"
#include <stdlib.h>

static int ft_setsockopt(t_ping *ping, socklen_t len) {
  if (setsockopt(ping->sockfd, SOL_SOCKET, SO_RCVTIMEO, &ping->timeout, len) ==
      -1) {
    fprintf(stderr, "ft_ping: error set setsockopt timeout: %s\n",
            strerror(errno));
    return EXIT_FAILURE;
  }
  if (setsockopt(ping->sockfd, IPPROTO_IP, IP_TTL, &ping->ttl,
                 sizeof(ping->ttl)) == -1) {
    fprintf(stderr, "ft_ping: error set setsockopt ttl: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static int ft_socket(t_ping *ping) {
  ping->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (ping->sockfd == -1) {
    fprintf(stderr, "ft_ping: error creation socket: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static void setaddr(t_ping *ping) {
  ping->dest_addr.sin_family = AF_INET;
  ping->dest_addr.sin_addr.s_addr = inet_addr(ping->ip);
  ping->icmp_header = (struct icmp *)ping->packet;
  ping->icmp_header->icmp_id = getpid() & 0xFFFF;
  ping->header(ping);
}

int openSocket(t_ping *ping) {
  socklen_t len;

  len = sizeof(ping->timeout);

  if (ft_socket(ping))
    return EXIT_FAILURE;
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
  if (ft_setsockopt(ping, len))
    return EXIT_FAILURE;
  setaddr(ping);
  return EXIT_SUCCESS;
}
