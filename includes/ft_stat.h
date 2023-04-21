#ifndef FT_STAT_H

#include <stdlib.h>
#define FT_STAT_H

#ifndef DATA
#define DATA 0
#endif
#ifndef DEV
#define DEV 1
#endif

typedef struct s_data {
  double data;
  struct s_data *next;
} t_data;

typedef struct s_stat {
  t_data *data;
  t_data *stddev;
  int (*insert)(struct s_stat *, double data, int flag);
  void (*free)(struct s_stat *);
  double (*min)(struct s_stat *, int flag);
  double (*max)(struct s_stat *, int flag);
  double (*avg)(struct s_stat *, int flag);
} t_stat;

void initStat(t_stat *stat);
int insert_data(t_stat *stat, double data, int flag);
double get_avg_data(t_stat *stat, int flag);
double get_max_data(t_stat *stat, int flag);
double get_min_data(t_stat *stat, int flag);
void free_data(t_stat *stat);
t_data *get_data(t_stat *stat, int flag);

#endif
