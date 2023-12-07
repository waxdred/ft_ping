/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmilhas <jmilhas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 19:54:00 by jmilhas           #+#    #+#             */
/*   Updated: 2023/12/07 09:50:11 by jmilhas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"
#include <stdio.h>

void closePing(t_ping *ping) {
  struct timeval end;

  gettimeofday(&end, NULL);
  printf("--- %s ping statistics ---\n", ping->hostname);
  if (ping->Error != 0) {
    printf("%d packets transmitted, %d packets receive, +%d errors, %.1lf%% "
           "packet loss\n",
           ping->seq, ping->seqRecv, ping->Error, getPourcente(ping));
  } else {
    printf("%d packets transmitted, %d packets receive, %.1lf%% packet loss\n",
           ping->seq, ping->seqRecv, getPourcente(ping));
  }
  if (ping->seqRecv != 0) {
    printf("round-trip min/avg/max/stddev = %.2lf/%.2lf/%.2lf/%.2lf ms\n",
           ping->stat.min(&ping->stat, DATA), ping->stat.avg(&ping->stat, DATA),
           ping->stat.max(&ping->stat, DATA), ping->stat.avg(&ping->stat, DEV));
  }
  ping->stat.free(&ping->stat);
  ping->free(ping);
  close(ping->sockfd);
}

void ft_process(t_ping *ping) {
  if (DEBUG_EXE) {
    debug((dprintf_func)dprintf, 2, "Run Process\n");
  }

  while (ping->signal) {
    struct timeval dev;
    ft_bzero(&dev, sizeof(struct timeval));
    if (!ping->flag.count.ok && ping->flag.count.value == ping->seqRecv) {
      break;
    }
    gettimeofday(&ping->tv, NULL);
    if (DEBUG_EXE) {
      debug((dprintf_func)dprintf, 2, "Setting timeval dev: %d\n", dev.tv_usec);
      debug((dprintf_func)dprintf, 2, "check W : %d\n", ping->flag.runtime.ok);
    }
    if (!ping->flag.runtime.ok &&
        !cmptv(ping->runtime, ping->tv, ping->flag.runtime.value))
      break;
    gettimeofday(&dev, NULL);
    if (ping->receive(ping, dev)) {
      break;
    };
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
