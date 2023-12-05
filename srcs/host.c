#include "../includes/ft_ping.h"
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>

static void get_info(t_ping *ping, struct addrinfo *servinfo) {
  struct addrinfo *tmp;
  char host[100];

  tmp = servinfo;
  ft_memset(host, 0, sizeof(host));
  while (tmp != NULL) {
    getnameinfo(tmp->ai_addr, tmp->ai_addrlen, host, sizeof(host), NULL, 0,
                NI_NUMERICHOST);
    ft_strcpy(ping->ip, host);
    if (DEBUG_EXE) {
      debug((dprintf_func)dprintf, 2, "Read host: %s\n", host);
    }
    tmp = tmp->ai_next;
  }
  freeaddrinfo(servinfo);
}

int host_to_ip(t_ping *ping) {
  int recv;
  struct addrinfo hint;
  struct addrinfo *servinfo;

  ft_bzero(&hint, sizeof(hint));
  hint.ai_family = AF_INET;
  recv = getaddrinfo(ping->hostname, NULL, &hint, &servinfo);
  if (recv < 0) {
    fprintf(stderr, "ft_ping: error getaddrinfo\n");
    return EXIT_FAILURE;
  } else if (servinfo == NULL) {
    fprintf(stderr, "ft_ping: cannot resolve: %s: Unknown host\n",
            ping->hostname);
    return EXIT_FAILURE;
  }
  get_info(ping, servinfo);
  return EXIT_SUCCESS;
}

int ft_cmp_address(t_ping *ping, t_recv *r) {
  r->ipRcv = inet_ntoa(r->from.sin_addr);
  return ft_strcmp(ping->ip, r->ipRcv);
}
