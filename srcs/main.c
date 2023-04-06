#include "../includes/ping.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main(int ac, char *av[]) {
  t_ping *ping;

  ping = initPing();
  if (ping == NULL)
    return EXIT_FAILURE;
  if (ac < 2) {
    // display commande
    ping->help();
    ping->free(ping, 0);
    exit(EXIT_FAILURE);
  }

  if (ping->parse(ping, ac, av)) {
    ping->free(ping, 0);
    return EXIT_FAILURE;
  }
  int err = 0;
  if ((err = ping->connection(ping)) != 0) {
    if (err == 2)
      ping->free(ping, 1);
    else
      ping->free(ping, 0);
    return EXIT_FAILURE;
  }
  ping->run(ping);
  return (0);
}
