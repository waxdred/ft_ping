#include "../includes/ft_ping.h"

void freePing(t_ping *ping) {
  if (ping->alloc == 1)
    free(ping->packet);
  close(ping->sockfd);
  free(ping);
}

void fill_seq_icmp(t_ping *ping) {
  ping->icmp_header->icmp_type = ICMP_ECHO;
  ping->icmp_header->icmp_code = 0;
  ping->icmp_header->icmp_id = getpid() & 0xFFFF;
  ping->icmp_header->icmp_seq = htons(ping->seq);
  memset(ping->icmp_header->icmp_data, '*', ping->packetSize);
  ping->icmp_header->icmp_cksum = 0;
  ping->icmp_header->icmp_cksum =
      calculate_checksum((unsigned short *)ping->icmp_header, ping->packetSize);
}

int ft_send(t_ping *ping) {
  ping->header(ping);
  if (sendto(ping->sockfd, ping->packet, ping->packetSize, 0,
             (struct sockaddr *)&ping->dest_addr,
             sizeof(ping->dest_addr)) < 0) {
    fprintf(stderr, "ft_ping: error sending packet: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }
  ping->seq++;
  return EXIT_SUCCESS;
}

static void print_data(t_ping *ping, t_recv *r, struct timeval dev) {
  r->data = (double)(r->end.tv_usec - ping->tv.tv_usec) / 1000;
  r->stddev = (double)(r->end.tv_usec - dev.tv_usec) / 1000;
  if (ping->flag.silence.ok == -1) {
#ifdef __OS__
    struct ip *ip = (struct ip *)r->buf;
    printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3lf ms\n", r->ret,
           ping->hostname, ping->ip, ping->seq, ip->ip_ttl, r->data);
#else
    struct iphdr *ip = (struct iphdr *)r->buf;
    printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3lf ms\n", r->ret,
           ping->hostname, ping->ip, ping->seq, ip->ttl, r->data);
#endif
  }
}

int ft_receive(t_ping *ping, struct timeval dev) {
  t_recv r;

  ft_bzero(&r, sizeof(t_recv));
  r.fromlen = sizeof(r.from);
  ft_bzero(r.buf, 1024);
  r.ret = recvfrom(ping->sockfd, r.buf, sizeof(r.buf), 0,
                   (struct sockaddr *)&r.from, &r.fromlen);
  if (r.ret < 0) {
    printf("Request timeout for imcp_seq %d\n", ping->seq);
    return EXIT_FAILURE;
  }
  gettimeofday(&r.end, NULL);
  print_data(ping, &r, dev);
  ping->seqRecv++;
  ping->stat.insert(&ping->stat, r.data, DATA);
  ping->stat.insert(&ping->stat, r.stddev, DEV);
  return EXIT_SUCCESS;
}
