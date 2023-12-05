#include "../includes/ft_ping.h"
#include <stdio.h>

int main(int ac, char *av[]) {
  t_ping *ping;

  ping = initPing(ac);
  if (ping == NULL)
    return EXIT_FAILURE;
  if (ac < 2) {
    ping->help(av[0]);
    ping->free(ping);
    exit(EXIT_FAILURE);
  }

  if (ping->parse(ping, ac, av)) {
    ping->free(ping);
    return EXIT_FAILURE;
  }
  int err = 0;
  if ((err = ping->connection(ping)) != 0) {
    ping->free(ping);
    return EXIT_FAILURE;
  }
  ping->run(ping);
  return (0);
}
