#include "../includes/ft_ping.h"
#include <stdio.h>
#include <stdlib.h>

static void ft_getopt(char *av, char *flag, int *opt) {
  const char *optchar;

  if (av[0] != '-') {
    *opt = -1;
    return;
  } else if (av[0] == '-' && av[2] != '\0') {
    *opt = -2;
    fprintf(stderr, "ft_ping: invalid arguments: '%s'\n", av);
    return;
  }
  *opt = av[1];
  optchar = ft_strchr(flag, *opt);
  if (optchar == NULL) {
    *opt = '?';
    return;
  }
}

static int ft_checkHQTW(t_ping *ping, int *i, char **av, char opt) {
  if ('t' == opt) {
    ping->flag.ttl.ok = 0;
    ping->flag.ttl.value = ft_atoi(av[*i + 1]);
    if (ping->flag.ttl.value <= 0) {
      fprintf(stderr, "ft_ping: invalid arguments: '%s'\n", av[*i + 1]);
      return EXIT_FAILURE;
    }
    ++*i;
    ping->ttl = ping->flag.ttl.value;
  } else if ('w' == opt) {
    ping->flag.runtime.ok = 0;
    ping->flag.runtime.value = ft_atoi(av[*i + 1]);
    if (ping->flag.runtime.value <= 0) {
      fprintf(stderr, "ft_ping: invalid arguments: '%s'\n", av[*i + 1]);
      return EXIT_FAILURE;
    }
    gettimeofday(&ping->runtime, NULL);
    ++i;
  }
  return EXIT_SUCCESS;
}

static int ft_checkC(t_ping *ping, int *i, char **av, char opt) {
  if ('h' == opt) {
    ping->help(av[0]);
    exit(0);
  } else if ('q' == opt) {
    ping->flag.silence.ok = 0;
  } else if ('c' == opt) {
    ping->flag.count.ok = 0;
    ping->flag.count.value = ft_atoi(av[*i + 1]);
    if (ping->flag.count.value <= 0) {
      fprintf(stderr, "ft_ping: invalid arguments: '%s'\n", av[*i + 1]);
      return EXIT_FAILURE;
    }
    ++*i;
  } else if ('?' == opt) {
    fprintf(stderr, "Unknown option `-%c'.\n", av[*i][1]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static int ft_checkW(t_ping *ping, int *i, char **av, char opt) {
  if ('W' == opt) {
    ping->flag.timeout.ok = 0;
    ping->flag.timeout.value = ft_atoi(av[*i + 1]);
    if (ping->flag.timeout.value <= 0) {
      fprintf(stderr, "ft_ping: invalid arguments: '%s'\n", av[*i + 1]);
      return EXIT_FAILURE;
    }
    ++*i;
    ping->timeout = (struct timeval){ping->flag.timeout.value, 0};
  }
  return EXIT_SUCCESS;
}

int parse(t_ping *ping, int ac, char **av) {
  int opt;
  int i;

  opt = 0;
  i = 1;
  while (i < ac) {
    ft_getopt(av[i], "hvtcWqw", &opt);
    if (opt == -2)
      return EXIT_FAILURE;
    if (ft_checkHQTW(ping, &i, av, opt) == EXIT_FAILURE) {
      return EXIT_FAILURE;
    } else if (ft_checkC(ping, &i, av, opt) == EXIT_FAILURE) {
      return EXIT_FAILURE;
    } else if (ft_checkW(ping, &i, av, opt) == EXIT_FAILURE) {
      return EXIT_FAILURE;
    } else if (opt == -1) {
      if (ping->hostname != NULL) {
        ping->help(av[0]);
        return EXIT_FAILURE;
      }
      ping->hostname = av[i];
    }
    ++i;
  }
  return EXIT_SUCCESS;
}
