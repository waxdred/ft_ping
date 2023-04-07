#ifndef FT_STAT_H

#include <stdlib.h>
#define FT_STAT_H

#ifndef DATA
#define DATA 0
#endif /* ifndef DATA */
#ifndef DEV
#define DEV 1
#endif /* ifndef DEV 1 */

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
#endif
