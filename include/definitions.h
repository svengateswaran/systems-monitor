#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#define PORT 8080

#ifndef PROCDIR
#define PROCDIR "/proc"
#endif

#ifndef PROCSTATFILE
#define PROCSTATFILE PROCDIR "/stat"
#endif

#ifndef PROCMEMINFOFILE
#define PROCMEMINFOFILE PROCDIR "/meminfo"
#endif

#ifndef PROC_LINE_LENGTH
#define PROC_LINE_LENGTH 4096
#endif

typedef struct {
  float cpu_load;
} sys_data;

#endif /* __DEFINITIONS_H__ */
