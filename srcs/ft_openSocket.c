#include "../includes/ft_ping.h"
#include <stdio.h>

static int ft_setsockopt(t_ping *ping) {
  if (ping->flag.timeout.ok == 0) {
    if (setsockopt(ping->sockfd, SOL_SOCKET, SO_RCVTIMEO, &ping->timeout,
                   sizeof(ping->timeout)) < 0) {
      fprintf(stderr, "ft_ping: error set setsockopt timeout: %s\n",
              strerror(errno));
      goto error;
    }
  }
  if (setsockopt(ping->sockfd, IPPROTO_IP, IP_TTL, &ping->ttl,
                 sizeof(ping->ttl)) < 0) {
    fprintf(stderr, "ft_ping: error set setsockopt ttl: %s\n", strerror(errno));
    goto error;
  }
  return EXIT_SUCCESS;
error:
  close(ping->sockfd);
  return EXIT_FAILURE;
}

static int ft_socket(t_ping *ping) {
  ping->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (ping->sockfd == -1) {
    fprintf(stderr, "ft_ping: error creation socket: %s\n", strerror(errno));
    close(ping->sockfd);
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
  if (ft_socket(ping))
    goto error;
  if (ping->flag.size.ok != 0) {
    ping->packetSize = sizeof(struct icmp) + sizeof(struct timeval);
  } else if (ping->flag.size.ok == 0 && ping->packetSize < 8) {
    ping->packetSize = 8;
  }
  ping->packet = (char *)malloc(ping->packetSize + 8);
  if (!ping->packet) {
    fprintf(stderr, "ft_ping: error allocation: %s\n", strerror(errno));
    return 2;
  }
  if (sizeof(struct icmp) > sizeof(ping->packetSize)) {
    ft_memset(ping->packet, 0, sizeof(ping->packetSize + 8));

  } else {
    ft_memset(ping->packet, 0, sizeof(struct icmp));
  }
  ping->alloc = 1;
  if (ping->getname(ping))
    goto error;
  if (ft_setsockopt(ping))
    goto error;
  setaddr(ping);
  return EXIT_SUCCESS;
error:
  close(ping->sockfd);
  return EXIT_FAILURE;
};
