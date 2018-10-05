#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#define DEBUG 0

#define PORT 7000

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

#define DATA_DIR       "site/data"
#define CLIENTS_LIST   "config/clients.txt"
#define CLIENT_LEN     32
#define FILE_PATH_LEN  4096
#define FILENAME_LEN   1024
#define GET_GB(x)      (x/(1024*1024*1024.0))

#if DEBUG
#define DEBUG_INFO(...) {printf(__VA_ARGS__); fflush(stdout);}
#else
#define DEBUG_INFO(...) ;
#endif

#define MAX_NUM_GPU 2
#define GPU_NAME_LEN 512

typedef struct {
  float cpu_util;

  /* GPU Info */
  unsigned int gpu_count;
  char gpu_name[MAX_NUM_GPU][GPU_NAME_LEN];
  unsigned int gpu_cores_util[MAX_NUM_GPU];
  unsigned int gpu_mem_util[MAX_NUM_GPU];
  unsigned long long gpu_mem_total[MAX_NUM_GPU];
  unsigned long long gpu_mem_used[MAX_NUM_GPU];
  unsigned int gpu_temperature[MAX_NUM_GPU];

} sys_data;

#endif /* __DEFINITIONS_H__ */
