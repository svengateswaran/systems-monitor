/******************************************************************************
 *
 * Simple Remote Systems Monitoring Tool
 *
 * Author : Vengateswaran <saivengatraghu@gmail.com>
 *
 * ***************************************************************************/

#include <nvml.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <definitions.h>

nvmlReturn_t (*nvmlDeviceGetUtilizationRatesPtr)(nvmlDevice_t, nvmlUtilization_t *);
nvmlReturn_t (*nvmlDeviceGetTemperaturePtr) (nvmlDevice_t, nvmlTemperatureSensors_t, unsigned int *);
nvmlReturn_t (*nvmlDeviceGetMemoryInfoPtr)(nvmlDevice_t, nvmlMemory_t *);

unsigned long long getMemoryInfo(nvmlDevice_t dev, int which_one, void *dl)
{
    nvmlDeviceGetMemoryInfoPtr = dlsym(dl, "nvmlDeviceGetMemoryInfo");
    if (dlerror() != NULL) {
        printf("NVML function nvmlDeviceGetMemoryInfo not found.");
        return -1;
    }

    nvmlMemory_t meminfo;
    nvmlReturn_t bad;
    bad = (*nvmlDeviceGetMemoryInfoPtr)(dev, &meminfo);

    if (NVML_SUCCESS != bad) {
        DEBUG_INFO("something went wrong with NVML\n");
    }

    switch (which_one) {
    case 0:
        return meminfo.total;
    case 1:
        return meminfo.free;
    case 2:
        return meminfo.used;
    default:
        ;
    }
    return (unsigned long long) - 1;
}

unsigned long long getTemperature(nvmlDevice_t dev, void *dl)
{
    nvmlDeviceGetTemperaturePtr = dlsym(dl, "nvmlDeviceGetTemperature");
    if (dlerror() != NULL) {
        printf("NVML function nvmlDeviceGetTemperature not found.");
        return -1;
    }

    unsigned int ret = 0;
    nvmlReturn_t bad;
    bad = (*nvmlDeviceGetTemperaturePtr)(dev, NVML_TEMPERATURE_GPU, &ret);

    if (NVML_SUCCESS != bad) {
       DEBUG_INFO("something went wrong with NVML\n");
    }
    return (unsigned long long)ret;
}

/*  0 => gpu util
 *  1 => memory util
 */
unsigned long long getUtilization(nvmlDevice_t dev, int which_one, void *dl)
{
    nvmlDeviceGetUtilizationRatesPtr = dlsym(dl, "nvmlDeviceGetUtilizationRates");
    if (dlerror() != NULL) {
        printf("NVML function nvmlDeviceGetUtilizationRates not found.");
        return 0;
    }

    nvmlUtilization_t util;
    nvmlReturn_t bad;
    bad = (*nvmlDeviceGetUtilizationRatesPtr)(dev, &util);

    if (NVML_SUCCESS != bad) {
        DEBUG_INFO("something went wrong with NVML ... \n");
    }

    switch (which_one) {
    case 0:
        return (unsigned long long) util.gpu;
    case 1:
        return (unsigned long long) util.memory;
    default:
        ;
    }

    return (unsigned long long) - 1;
}

unsigned int get_gpu_count() {
    nvmlReturn_t result;
    unsigned int device_count;
    result = nvmlDeviceGetCount(&device_count);
    if (NVML_SUCCESS != result)
    { 
        DEBUG_INFO("Failed to query device count: %s\n", nvmlErrorString(result));
        return -1;
    }
    return device_count;
}

void* init_gpu_stat() {
    nvmlReturn_t result;
    unsigned int device_count, i;

    // First initialize NVML library
    result = nvmlInit();
    if (NVML_SUCCESS != result)
    { 
        DEBUG_INFO("Failed to initialize NVML: %s\n", nvmlErrorString(result));
        return NULL;
    }

    static void* dl = NULL;
    dl = dlopen("libnvidia-ml.so", RTLD_NOW | RTLD_GLOBAL);
    if (!dl) {
        printf("libnvml not found\n");
        return NULL;
    }
    return dl;
}

int get_gpu_stat(int index, void *dl, char *device_name, unsigned int *gpu_util,
                 unsigned int *mem_util, unsigned int *temperature,
                 unsigned long long *mem_total, unsigned long long *mem_used) {

   nvmlReturn_t result;
   nvmlDevice_t device;
   char name[NVML_DEVICE_NAME_BUFFER_SIZE];
   nvmlPciInfo_t pci;
   nvmlComputeMode_t compute_mode;

   // Query for device handle to perform operations on a device
   // You can also query device handle by other features like:
   // nvmlDeviceGetHandleBySerial
   // nvmlDeviceGetHandleByPciBusId
   result = nvmlDeviceGetHandleByIndex(index, &device);
   DEBUG_INFO("device handle success\n");
   if (NVML_SUCCESS != result)
   {
       DEBUG_INFO("Failed to get handle for device %i: %s\n", index, nvmlErrorString(result));
       return -1;
   }

   result = nvmlDeviceGetName(device, name, NVML_DEVICE_NAME_BUFFER_SIZE);
   DEBUG_INFO("device name success\n");
   if (NVML_SUCCESS != result)
   {
       DEBUG_INFO("Failed to get name of device %i: %s\n", index, nvmlErrorString(result));
       return -1;
   }
 
   strcpy(device_name, name);
   *gpu_util    = getUtilization(device, 0, dl);
   *mem_util    = getUtilization(device, 1, dl); 
   *mem_total   = getMemoryInfo(device, 0, dl);
   *mem_used    = getMemoryInfo(device, 2, dl);
   *temperature = getTemperature(device, dl);
   DEBUG_INFO("%d. %s - p:%llu%% m:%llu%% t:%llu mt:%llu mf:%llu mu:%llu\n",
              index, name, getUtilization(device, 0, dl), getUtilization(device, 1, dl), getTemperature(device, dl),
              getMemoryInfo(device, 0, dl)/(1024*1024), getMemoryInfo(device, 1, dl)/(1024*1024) ,
              getMemoryInfo(device, 2, dl)/(1024*1024));

   return 0;
}
