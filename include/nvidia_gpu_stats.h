#ifndef __NVIDIA_GPU_STAT_H__
#define __NVIDIA_GPU_STAT_H__

void* init_gpu_stat();

unsigned int get_gpu_count();

int get_gpu_stat(int index, void *dl, char *device_name, unsigned int *gpu_util,
                 unsigned int *mem_util, unsigned int *temperature,
                 unsigned long long *mem_total, unsigned long long *mem_used);


#endif
