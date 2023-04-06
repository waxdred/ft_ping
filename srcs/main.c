#include "../includes/ping.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int signalStop;

void handle_signal() {
  signalStop = 0;
}

int main(int ac, char *av[]) {
  t_ping *ping;
  signalStop = 1;
  signal(SIGINT, handle_signal);

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
  printf("PING %s (%s): %d data bytes\n", ping->hostname,
         inet_ntoa(ping->dest_addr.sin_addr), ping->pacetSize);
  while (signalStop) {
    sleep(2);
    gettimeofday(&ping->tv, NULL);
    if (ping->send(ping)) {
      perror("Error sendto");
      ping->free(ping, 1);
      return EXIT_FAILURE;
    }
    if (ping->receive(ping)) {
      ping->free(ping, 1);
      return EXIT_FAILURE;
    }
  }
  ping->close(ping);
  return (0);
}
