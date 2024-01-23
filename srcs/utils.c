#include "../includes/ft_ping.h"
#include <ctype.h>

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

int ft_isdigit(const char *str) {
  int i = 0;
  while (str[i]) {
    if (str[i] < 48 || str[i] > 57) {
      return 0;
    }
    i++;
  }
  return 1;
}

void *ft_memcpy(void *dst, const void *src, size_t n) {
  unsigned char *pdst;
  unsigned char *psrc;

  pdst = (unsigned char *)dst;
  psrc = (unsigned char *)src;
  while (n > 0) {
    *pdst++ = *psrc++;
    n--;
  }
  return (dst);
}

char *ft_trimNb(char *str) {
  int i = 0;
  int j = 0;
  while (str[i] && isdigit(str[i])) {
    i++;
  }
  while (str[i]) {
    str[j] = str[i];
    i++;
    j++;
  }
  str[j] = '\0';
  return (str);
}

int ft_atoi(const char *str) {
  long result;
  long sign;

  result = 0;
  sign = 1;
  if (*str == '-') {
    sign = -1;
    str++;
  } else if (*str == '+')
    str++;
  while (*str >= '0' && *str <= '9') {
    if (result > INT_MAX)
      return (-1);
    result = result * 10 + (*str - '0');
    str++;
  }
  return ((int)(result * sign));
}
