#include "../includes/ft_stat.h"
#include <stdio.h>

t_data *get_data(t_stat *stat, int flag) {
  t_data *ret;
  if (flag == DATA)
    ret = stat->data;
  else
    ret = stat->stddev;
  return ret;
}

void free_data(t_stat *stat) {
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

double get_min_data(t_stat *stat, int flag) {
  t_data *it;
  double ret;

  ret = 0;
  it = get_data(stat, flag);

  while (it) {
    if (it->data < ret || ret == 0)
      ret = it->data;
    it = it->next;
  }
  return ret;
}

double get_max_data(t_stat *stat, int flag) {
  t_data *it;
  double ret;

  ret = 0;
  it = get_data(stat, flag);
  while (it) {
    if (it->data > ret)
      ret = it->data;
    it = it->next;
  }
  return ret;
}

double get_avg_data(t_stat *stat, int flag) {
  t_data *it;
  double ret;
  int i;

  ret = 0;
  i = 0;
  it = get_data(stat, flag);
  while (it) {
    ret += it->data;
    i++;
    it = it->next;
  }
  return ret / i;
}
