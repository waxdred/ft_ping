#include "../includes/ft_ping.h"

void set_func_ptr(t_ping *ping) {
  ping->help = &help;
  ping->free = &freePing;
  ping->parse = &parse;
  ping->connection = &openSocket;
  ping->send = &ft_send;
  ping->receive = &ft_receive;
  ping->close = &closePing;
  ping->getname = &host_to_ip;
  ping->run = &run_ping;
}

t_ping *initPing(int ac) {
  t_ping *ping = malloc(sizeof(t_ping));

  if (ping == NULL)
    return NULL;
  bzero(ping, sizeof(t_ping));
  initStat(&ping->stat);
  memset(&ping->flag, -1, sizeof(t_flag));
  set_func_ptr(ping);
  ping->header = fill_seq_icmp;
  ping->ttl = 64;
  ping->ac = ac;
  ping->timeout = (struct timeval){1, 0};
  get_ping(ping);
  return (ping);
}
