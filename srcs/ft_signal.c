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

void handler_alarm(int sig) {
  t_ping *ping;

  if (DEBUG_EXE) {
    debug((dprintf_func)dprintf, 2, "%salarm%s: new signal\n", ANSI_COLOR_RED,
          ANSI_COLOR_RESET);
  }
  if (sig != SIGALRM)
    return;
  ping = get_ping(NULL);
  if (ping->send(ping)) {
    if (DEBUG_EXE) {
      debug((dprintf_func)dprintf, 2, "ft_ping: error sendto\n");
    }
  }
  alarm(1);
}
