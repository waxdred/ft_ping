#include "../includes/ft_ping.h"

void closePing(t_ping *ping) {
  struct timeval end;

  gettimeofday(&end, NULL);
  printf("--- %s ping statistics ---\n", ping->hostname);
  printf(
      "%d packets transmitted, %d packets receive, %.1lf%% packet loss, time "
      "%.3lfms\n",
      ping->seq, ping->seqRecv, getPourcente(ping),
      (double)(end.tv_usec - ping->start.tv_usec) / 1000);
  if (ping->seqRecv != 0) {
    printf("round-trip min/avg/max/stddev = %.2lf/%.2lf/%.2lf/%.2lf\n",
           ping->stat.min(&ping->stat, DATA), ping->stat.avg(&ping->stat, DATA),
           ping->stat.max(&ping->stat, DATA), ping->stat.avg(&ping->stat, DEV));
  }
  ping->stat.free(&ping->stat);
  ping->free(ping);
  close(ping->sockfd);
}

void ft_process(t_ping *ping) {
  while (ping->signal) {
    struct timeval dev;
    ft_bzero(&dev, sizeof(struct timeval));
    if (!ping->flag.count.ok && ping->flag.count.value == ping->seqRecv) {
      break;
    }
    gettimeofday(&ping->tv, NULL);
    if (!ping->flag.count.ok && ping->flag.count.value == ping->seqRecv) {
      if (!ping->flag.runtime.ok &&
          !cmptv(ping->runtime, ping->tv, ping->flag.runtime.value))
        break;
    }
    gettimeofday(&dev, NULL);
    ping->receive(ping, dev);
    usleep(2000000);
  }
}

int run_ping(t_ping *ping) {
  signal(SIGINT, handle_signal);
  signal(SIGALRM, handler_alarm);
  printf("PING %s (%s): %d data bytes\n", ping->hostname, ping->ip,
         ping->packetSize);
  gettimeofday(&ping->start, NULL);
  handler_alarm(SIGALRM);
  ping->signal = 1;
  ft_process(ping);
  ping->close(ping);
  return EXIT_SUCCESS;
}
