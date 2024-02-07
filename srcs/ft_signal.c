#include "../includes/ft_ping.h"
#include <stdio.h>
#include <stdlib.h>

t_ping *get_ping(t_ping *ping) {
  static t_ping *p;

  if (ping != NULL)
    p = ping;
  return p;
}

void handle_signal(int sig) {
  t_ping *ping;

  if (sig == SIGINT) {
    ping = get_ping(NULL);
    ping->signal = 0;
    ping->close(ping);
  }
  exit(0);
}
