#include "../includes/ft_ping.h"

static void get_info(t_ping *ping, struct addrinfo *servinfo) {
  struct addrinfo *tmp;
  char host[100];

  tmp = servinfo;
  while (tmp != NULL) {
    getnameinfo(tmp->ai_addr, tmp->ai_addrlen, host, sizeof(host), NULL, 0,
                NI_NUMERICHOST);
    strcpy(ping->ip, host);
    tmp = tmp->ai_next;
  }
  freeaddrinfo(servinfo);
}

int host_to_ip(t_ping *ping) {
  int recv;
  struct addrinfo hint;
  struct addrinfo *servinfo;

  bzero(&hint, sizeof(hint));
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
