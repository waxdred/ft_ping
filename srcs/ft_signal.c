#include "../includes/ft_ping.h"

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
  }
}

void handler_alarm(int sig) {
  t_ping *ping;

  if (sig != SIGALRM)
    return;
  ping = get_ping(NULL);
  if (ping->send(ping)) {
    fprintf(stderr, "ft_ping: error sendto\n");
  }
  alarm(1);
}
