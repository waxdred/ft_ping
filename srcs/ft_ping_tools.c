/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmilhas <jmilhas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 09:45:47 by jmilhas           #+#    #+#             */
/*   Updated: 2023/12/05 20:19:49 by jmilhas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <string.h>

void freePing(t_ping *ping) {
  if (ping->alloc == 1)
    free(ping->packet);
  close(ping->sockfd);
  free(ping);
}

void fill_seq_icmp(t_ping *ping) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  ping->icmp_header->icmp_type = ICMP_ECHO;
  ping->icmp_header->icmp_code = 0;
  ping->icmp_header->icmp_id = getpid() & 0xFFFF;
  ping->icmp_header->icmp_seq = htons(ping->seq);
  if (DEBUG_EXE) {
    debug((dprintf_func)dprintf, 2, "Packet size: %d\n", ping->packetSize);
  }
  memcpy(ping->icmp_header->icmp_data, "********", ping->packetSize);
  ping->icmp_header->icmp_cksum = 0;
  ping->icmp_header->icmp_cksum =
      calculate_checksum((unsigned short *)ping->icmp_header, ping->packetSize);
}

int ft_send(t_ping *ping) {
  struct timeval tv;
  int ret;
  gettimeofday(&tv, NULL);

  ping->header(ping);
  ping->tv = tv;
  if (DEBUG_EXE) {
    debug((dprintf_func)dprintf, 2, "Ping send: %d\n", ping->seq);
  }
  ret = sendto(ping->sockfd, ping->packet, ping->packetSize, 0,
               (struct sockaddr *)&ping->dest_addr, sizeof(ping->dest_addr));
  if (DEBUG_EXE) {
    debug((dprintf_func)dprintf, 2, "Ret sendto: %d\n", ret);
  }
  if (ret < 0) {
    fprintf(stderr, "ft_ping: sendto: %s\n", strerror(errno));
    ping->Error++;
    return EXIT_FAILURE;
  }
  ping->seq++;
  return EXIT_SUCCESS;
}

static void print_data(t_ping *ping, t_recv *r, struct timeval dev, int seq) {
  r->data = (double)(r->end.tv_usec - ping->tv.tv_usec) / 1000;
  r->stddev = (double)(r->end.tv_usec - dev.tv_usec) / 1000;
  if (ping->flag.silence.ok == -1) {
#ifdef __OS__
    struct ip *ip = (struct ip *)r->buf;
#else
    struct iphdr *ip = (struct iphdr *)r->buf;
#endif
    if (r->ttsError == 0) {
      dprintf(1, "%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3lf ms\n",
              r->ret, ping->hostname, r->ipRcv, seq, ip->ip_ttl, r->data);
    } else {
      dprintf(
          1, "%d bytes from _gateway (%s): icmp_seq=%d Time to live exceeded\n",
          r->ret, r->ipRcv, ping->seq);
    }
    if (ping->packetSize >= 78) {
      dprintf(1, "wrong total length 96 instead of 97\n");
    }
  }
}

int ft_receive(t_ping *ping, struct timeval dev) {
  t_recv r;

  ft_bzero(&r, sizeof(t_recv));
  r.fromlen = sizeof(r.from);
  ft_bzero(r.buf, 1024);
  if (DEBUG_EXE) {
    debug((dprintf_func)dprintf, 2, "Waiting to receive: seq %d\n", ping->seq);
  }

  r.ret = recvfrom(ping->sockfd, r.buf, sizeof(r.buf), 0,
                   (struct sockaddr *)&r.from, &r.fromlen);
  if (r.ret < 0) {
    ping->Error++;
    if (ping->flag.silence.ok) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        dprintf(2, "Request timeout for icmp_seq %d\n", ping->seq);
      } else {
        dprintf(2, "ft_ping: error receiving packet: %s\n", strerror(errno));
      }
    }
    return EXIT_SUCCESS;
  }
#ifdef __OS__
  struct icmp *ipcmp = (struct icmp *)(r.buf + sizeof(struct ip));
#else
  struct icmphdr *icmp = (struct icmphdr *)(r.buf + sizeof(struct iphdr));
#endif
  gettimeofday(&r.end, NULL);
  ft_cmp_address(ping, &r);
  switch (ipcmp->icmp_type) {
  case ICMP_ECHOREPLY:
    ping->seqRecv++;
    print_data(ping, &r, dev, ntohs(ipcmp->icmp_seq));
    ping->stat.insert(&ping->stat, r.data, DATA);
    ping->stat.insert(&ping->stat, r.stddev, DEV);
    break;
  case ICMP_TIMXCEED:
    r.ipRcv = inet_ntoa(r.from.sin_addr);
    r.ttsError = 1;
    print_data(ping, &r, dev, ntohs(ipcmp->icmp_seq));
    ping->stat.insert(&ping->stat, r.data, DATA);
    ping->stat.insert(&ping->stat, r.stddev, DEV);
    ping->Error++;
    break;
  default:
    dprintf(2, "Not reconise");
    break;
  }
  if (DEBUG_EXE) {
    debug((dprintf_func)dprintf, 2, "ICMP sequence: %d\n",
          ntohs(ipcmp->icmp_seq));
    debug((dprintf_func)dprintf, 2, "ICMP type: %d\n", ipcmp->icmp_type);
    debug((dprintf_func)dprintf, 2, "Addres receive: %d\n", r.ret);
    debug((dprintf_func)dprintf, 2, "Time dev init: %3.lf\n",
          (double)dev.tv_usec);
    debug((dprintf_func)dprintf, 2, "Time rcv init: %3.lf\n",
          (double)r.end.tv_usec);
    debug((dprintf_func)dprintf, 2, "Time get diff: %3.lf\n",
          get_diff_tv(r.end, ping->tv));
    debug((dprintf_func)dprintf, 2, "Buff: %s\n", r.buf);
  }
  return EXIT_SUCCESS;
}
