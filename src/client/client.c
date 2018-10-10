/******************************************************************************
 *
 * Simple Remote Systems Monitoring Tool
 *
 * Author : Vengateswaran <saivengatraghu@gmail.com>
 *
 * ***************************************************************************/

#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <definitions.h> 

#include <linux_stats.h>
#include <nvidia_gpu_stats.h>

int main() {
  int client_fd, new_socket, valread; 
  struct sockaddr_in address; 
  int opt = 1; 
  int addrlen = sizeof(address); 

  void *nvml_dl = init_gpu_stat();
  int gpu_index = 0;

  attach:
  /* Creating socket file descriptor */
  if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
  { 
      perror("socket creation failed"); 
      exit(EXIT_FAILURE); 
  }

  DEBUG_INFO("socket created\n");
  /* Forcefully attaching socket to the port */
  if (setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                &opt, sizeof(opt)))
  {
      perror("setsockopt failed");
      exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( PORT );

  DEBUG_INFO("setsocketopt successfull\n");
  /* Forcefully attaching socket to the port */
  if (bind(client_fd, (struct sockaddr *)&address,
                               sizeof(address))<0)
  {
      perror("bind failed");
      exit(EXIT_FAILURE);
  }
  DEBUG_INFO("bind successfull\n");
  if (listen(client_fd, 5) < 0)
  {
      perror("listen");
      exit(EXIT_FAILURE);
  }
  DEBUG_INFO("listen successfull\n");
  if ((new_socket = accept(client_fd, (struct sockaddr *)&address,
                     (socklen_t*)&addrlen))<0)
  {
      perror("accept");
      exit(EXIT_FAILURE);
  }
  DEBUG_INFO("connected to server\n");

 
  while(1) {
    sys_data *data = (sys_data*) malloc(sizeof(sys_data));
    data->cpu_util = GetCPULoad();

    GetDiskUsage(&data->disk_freespace, &data->disk_capacity);
 
    if(nvml_dl) {
      data->gpu_count = get_gpu_count();
 
      for (gpu_index = 0; gpu_index < data->gpu_count; gpu_index++) {
        if(get_gpu_stat(gpu_index, nvml_dl,
                        data->gpu_name[gpu_index],
                        &data->gpu_cores_util[gpu_index],
                        &data->gpu_mem_util[gpu_index],
                        &data->gpu_temperature[gpu_index],
                        &data->gpu_mem_total[gpu_index],
                        &data->gpu_mem_used[gpu_index]) < 0) {
           DEBUG_INFO("NVML Error!\n");
           data->gpu_count = 911; /* Emergency ;) */
        }
      }                   
    } else {
      data->gpu_count = 911; /* Emergency */
    }
    if(send(new_socket , data , sizeof(sys_data), MSG_NOSIGNAL) != sizeof(sys_data)) {
      close(new_socket);
      close(client_fd);
      sleep(1);
      DEBUG_INFO("re-connecting to server ...\n");
      goto attach;
    }

    DEBUG_INFO("CPU Util : %.2f%%\n", data->cpu_util);

    free(data);
  }
  return 0;
}
