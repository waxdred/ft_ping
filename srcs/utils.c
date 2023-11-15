#include "../includes/ft_ping.h"

double get_diff_tv(struct timeval tv_recv, struct timeval tv_send) {
  struct timeval tv_diff;

  tv_diff.tv_sec = tv_recv.tv_sec - tv_send.tv_sec;
  tv_diff.tv_usec = tv_recv.tv_usec - tv_send.tv_usec;
  return ((double)tv_diff.tv_sec * 1000. + (double)tv_diff.tv_usec / 1000.);
}
