#include "../includes/stat.h"

static int insert_data(t_stat *stat, double data, int flag) {
  t_data *add = malloc(sizeof(t_data));

  if (!add)
    return EXIT_FAILURE;
  add->data = data;
  add->next = NULL;
  if (flag == DATA && stat->data == NULL) {
    stat->data = add;
    return EXIT_SUCCESS;
  } else if (flag == DEV && stat->stddev == NULL) {
    stat->stddev = add;
    return EXIT_SUCCESS;
  }
  for (t_data *it = flag == DATA ? stat->data : stat->stddev; it;
       it = it->next) {
    if (it->next == NULL) {
      it->next = add;
      break;
    }
  }
  return EXIT_SUCCESS;
}

static void free_data(t_stat *stat) {
  t_data *tmp;
  while (stat->data != NULL) {
    tmp = stat->data;
    stat->data = stat->data->next;
    free(tmp);
  }
  while (stat->stddev != NULL) {
    tmp = stat->stddev;
    stat->stddev = stat->stddev->next;
    free(tmp);
  }
}

static double get_min_data(t_stat *stat, int flag) {
  int i = 0;
  double ret = 0;

  for (t_data *it = flag == DATA ? stat->data : stat->stddev; it;
       it = it->next) {
    if (i == 0)
      ret = it->data;
    else if (it->data < ret)
      ret = it->data;
  }
  return ret;
}
static double get_max_data(t_stat *stat, int flag) {
  double ret = 0;

  for (t_data *it = flag == DATA ? stat->data : stat->stddev; it;
       it = it->next) {
    if (it->data > ret)
      ret = it->data;
  }
  return ret;
}

#include <stdio.h>
static double get_avg_data(t_stat *stat, int flag) {
  double ret = 0;
  ssize_t i = 0;

  for (t_data *it = flag == DATA ? stat->data : stat->stddev; it;
       it = it->next) {
    if (i == 0)
      ret = it->data;
    else
      ret += it->data;
    i++;
  }
  return ret / i;
}

void initStat(t_stat *stat) {
  stat->insert = &insert_data;
  stat->free = &free_data;
  stat->min = &get_min_data;
  stat->max = &get_max_data;
  stat->avg = &get_avg_data;
  return;
}
