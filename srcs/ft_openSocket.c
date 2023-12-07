#include "../includes/ft_ping.h"

static int ft_setsockopt(t_ping *ping) {
  if (setsockopt(ping->sockfd, SOL_SOCKET, SO_RCVTIMEO, &ping->timeout,
                 sizeof(ping->timeout)) < 0) {
    fprintf(stderr, "ft_ping: error set setsockopt timeout: %s\n",
            strerror(errno));
    goto error;
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
  ping->packetSize = sizeof(struct icmp) + sizeof(struct timeval);
  ping->packet = (char *)malloc(ping->packetSize);
  if (!ping->packet) {
    fprintf(stderr, "ft_ping: error allocation: %s\n", strerror(errno));
    return 2;
  }
  ft_memset(ping->packet, 0, sizeof(struct icmp));
  ping->alloc = 1;
  if (ping->getname(ping))
    goto error;
  if (ft_setsockopt(ping))
    goto error;
  setaddr(ping);
  if (ping->flag.verbose.ok == 0) {
    if (ping->dest_addr.sin_family == AF_INET) {
      dprintf(1, "ping: sock4.fd: %d (socktype: SOCK_RAW, family: %s)\n",
              ping->sockfd, "AF_INET");
    } else {
      dprintf(1, "ping: sock6.fd: %d (socktype: SOCK_RAW, family: %s)\n",
              ping->sockfd, "AF_INET6");
    }
  }
  return EXIT_SUCCESS;
error:
  close(ping->sockfd);
  return EXIT_FAILURE;
};
