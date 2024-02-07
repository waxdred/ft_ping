/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexadump.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmilhas <jmilhas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 09:45:47 by jmilhas           #+#    #+#             */
/*   Updated: 2023/12/05 20:19:49 by jmilhas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>

void PrintVerboseHexadump(t_recv r, t_ping *ping) {
  struct icmp *ipcmp = (struct icmp *)(r.buf + sizeof(struct ip));
  struct ip *ip = &ipcmp->icmp_ip;
  size_t hlen;
  unsigned char *cp;

  hlen = ip->ip_hl << 2;
  cp = (unsigned char *)ip + sizeof(*ip); /* point to options */

  if (ping->flag.verbose.ok == 0) {
    size_t j;

    dprintf(1, "IP Hdr Dump:\n ");
    for (j = 0; j < sizeof(*ip); ++j)
      dprintf(1, "%02x%s", *((unsigned char *)ip + j),
              (j % 2) ? " " : ""); /* Group bytes two by two.  */
    printf("\n");
    dprintf(1,
            "Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src\tDst\tData\n");
    dprintf(1, " %1x  %1x  %02x", ip->ip_v, ip->ip_hl, ip->ip_tos);
    dprintf(1, " %04x %04x",
            (ip->ip_len > 0x2000) ? ntohs(ip->ip_len) : ip->ip_len,
            ntohs(ip->ip_id));
    dprintf(1, "   %1x %04x", (ntohs(ip->ip_off) & 0xe000) >> 13,
            ntohs(ip->ip_off) & 0x1fff);
    dprintf(1, "  %02x  %02x %04x", ip->ip_ttl, ip->ip_p, ntohs(ip->ip_sum));
    dprintf(1, " %s ", inet_ntoa(*((struct in_addr *)&ip->ip_src)));
    dprintf(1, " %s ", inet_ntoa(*((struct in_addr *)&ip->ip_dst)));
    while (hlen-- > sizeof(*ip))
      dprintf(1, "%02x", *cp++);
    dprintf(1, "\n");
    dprintf(1, "ICMP: type %u, code %u, size %lu", ipcmp->icmp_type,
            ipcmp->icmp_code, ntohs(ip->ip_len) - hlen);
    dprintf(1, ", id 0x%04x, seq 0x%04x", getpid(),
            *(cp + 6) * 256 + *(cp + 7));
    dprintf(1, "\n");
  }
}
