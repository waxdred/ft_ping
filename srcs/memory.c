#include "../includes/ft_ping.h"

void ft_bzero(void *s, size_t n) {
  unsigned char *array;
  size_t index;

  array = s;
  index = 0;
  while (index < n) {
    array[index] = 0;
    index++;
  }
}

void *ft_memset(void *b, int c, size_t len) {
  unsigned char *pb;
  size_t i;

  pb = (unsigned char *)b;
  i = 0;
  while (i < len)
    pb[i++] = (unsigned char)c;
  return (b);
}

int ft_strlen(const char *s) {
  int i;

  i = 0;
  while (s[i] != '\0') {
    i++;
  }
  return i;
}

char *ft_strcpy(char *dest, const char *src) {
  int i;

  i = 0;
  while (src[i] != '\0') {
    dest[i] = src[i];
    i++;
  }
  return (dest);
}
