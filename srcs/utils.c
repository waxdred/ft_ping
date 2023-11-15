#include "../includes/ft_ping.h"

double get_diff_tv(struct timeval tv_recv, struct timeval tv_send) {
  struct timeval tv_diff;

  tv_diff.tv_sec = tv_recv.tv_sec - tv_send.tv_sec;
  tv_diff.tv_usec = tv_recv.tv_usec - tv_send.tv_usec;
  return ((double)tv_diff.tv_sec * 1000. + (double)tv_diff.tv_usec / 1000.);
}

void debug(dprintf_func func, int fd, const char *format, ...) {
  va_list args;
  va_start(args, format);

  func(fd, "%s[DEBUG]%s: ", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
  vfprintf(stderr, format, args);
  va_end(args);
}
