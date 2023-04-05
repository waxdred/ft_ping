#include "../includes/ping.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int signalStop;

void handle_signal(int signal) {
  printf("\nSignal %d reçu. Sortie.\n", signal);
  signalStop = 0;
  exit(0);
}

int main(int ac, char *av[]) {
  t_ping *ping;
  int opt = 0;
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

  while ((opt = getopt(ac, av, "hv")) != -1) {
    switch (opt) {
    case 'h':
      ping->help();
    case 'v':
      ping->verose = 1;
    }
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
  printf("PING %s (%s): %d data bytes\n", ping->adress,
         inet_ntoa(ping->dest_addr.sin_addr), ping->pacetSize);
  while (signalStop) {
    sleep(2);
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
  ping->free(ping, 1);
  return (0);
}
