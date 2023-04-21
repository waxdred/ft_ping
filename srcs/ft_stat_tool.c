#include "../includes/ft_stat.h"

int insert_null(t_stat *stat, t_data *add, int flag) {
  if (flag == DATA && stat->data == NULL) {
    stat->data = add;
    return EXIT_SUCCESS;
  } else if (flag == DEV && stat->stddev == NULL) {
    stat->stddev = add;
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

int insert_data(t_stat *stat, double data, int flag) {
  t_data *add;
  t_data *it;

  add = malloc(sizeof(t_data));
  it = get_data(stat, flag);
  if (!add)
    return EXIT_FAILURE;
  add->data = data;
  add->next = NULL;
  if (insert_null(stat, add, flag) == EXIT_SUCCESS) {
    return EXIT_SUCCESS;
  }
  while (it) {
    if (it->next == NULL) {
      it->next = add;
      break;
    }
    it = it->next;
  }
  return EXIT_SUCCESS;
}

void initStat(t_stat *stat) {
  stat->insert = &insert_data;
  stat->free = &free_data;
  stat->min = &get_min_data;
  stat->max = &get_max_data;
  stat->avg = &get_avg_data;
  return;
}
