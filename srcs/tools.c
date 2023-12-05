#include "../includes/ft_ping.h"

unsigned short calculate_checksum(void *addr, size_t count) {
  unsigned short *ptr;
  unsigned long sum;

  ptr = addr;
  sum = 0;
  while (count > 1) {
    sum += *ptr++;
    count -= 2;
  }
  if (count > 0)
    sum += *(unsigned char *)ptr;
  while (sum >> 16)
    sum = (sum & 0xffff) + (sum >> 16);
  return (~sum);
}

void formatDataPacket(void *data, char *timestamp, char *seq, int size) {
  int i = 0;
  int len_stamp = strlen(timestamp);
  /* int len_seq = strlen(seq); */
  unsigned char *ptr = (unsigned char *)data;

  while (i < len_stamp && i < size) {
    ptr[i] = timestamp[i];
    i++;
  }
  ptr[i] = ' ';
  i++;
  while (i < len_stamp && i < size) {
    ptr[i] = seq[i];
    i++;
  }
}

double getPourcente(t_ping *ping) {
  if (ping->seqRecv == 0)
    return 100;
  double ret = ping->seq - ping->seqRecv;
  return ((ret / ping->seq) * 100 / 100);
}

int8_t cmptv(struct timeval tv1, struct timeval tv2, int sec) {
  time_t t1;
  time_t t2;
  suseconds_t usec1;
  suseconds_t usec2;

  t1 = tv1.tv_sec;
  t2 = tv2.tv_sec;
  usec1 = tv1.tv_usec;
  usec2 = tv2.tv_usec;

  if (t1 + sec > t2) {
    return EXIT_FAILURE;
  } else if (t1 + sec < t2) {
    return EXIT_SUCCESS;
  } else {
    if (usec1 > usec2)
      return EXIT_SUCCESS;
    else
      return EXIT_FAILURE;
  }
}

int ft_atoi(const char *str) {
  int res;
  int i;
  int sign;

  i = 0;
  sign = 1;
  res = 0;
  while (str[i] == '\t' || str[i] == '\n' || str[i] == '\r' || str[i] == '\v' ||
         str[i] == '\f' || str[i] == ' ')
    i++;
  if (str[i] == '-' || str[i] == '+') {
    if (str[i] == '-')
      sign = -1;
    i++;
  }
  while (str[i] >= '0' && str[i] <= '9') {
    res *= 10;
    res += str[i] - '0';
    i++;
  }
  return (res * sign);
}

char *ft_strchr(const char *str, int c) {
  size_t i;

  i = 0;
  while (str[i]) {
    if (str[i] == c)
      return ((char *)str + i);
    i++;
  }
  if (str[i] == c)
    return ((char *)str + i);
  return (NULL);
}

int ft_strcmp(const char *s1, const char *s2){
    size_t i;
    i =0 ;
    while((unsigned char)s1[i] == (unsigned char)s2[i] && s1[i] != '\0' && s2[i] != '\0'){
        i++;
    }
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
