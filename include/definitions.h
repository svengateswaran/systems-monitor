#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#define DEBUG 0

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

#define DATA_DIR     "site/data"
#define CLIENTS_LIST "config/clients.txt"
#define CLIENT_LEN   32

#if DEBUG
#define DEBUG_INFO(...) {printf(__VA_ARGS__); fflush(stdout);}
#else
#define DEBUG_INFO(...) ;
#endif

typedef struct {
  float cpu_load;
} sys_data;

#endif /* __DEFINITIONS_H__ */
