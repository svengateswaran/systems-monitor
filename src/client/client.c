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

int main() {
  int client_fd, new_socket, valread; 
  struct sockaddr_in address; 
  int opt = 1; 
  int addrlen = sizeof(address); 
  char buffer[1024] = {0}; 
  char *hello = "Hello from client"; 
     
  /* Creating socket file descriptor */
  if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
  { 
      perror("socket creation failed"); 
      exit(EXIT_FAILURE); 
  }

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

  /* Forcefully attaching socket to the port */
  if (bind(client_fd, (struct sockaddr *)&address,
                               sizeof(address))<0)
  {
      perror("bind failed");
      exit(EXIT_FAILURE);
  }
  if (listen(client_fd, 3) < 0)
  {
      perror("listen");
      exit(EXIT_FAILURE);
  }
  if ((new_socket = accept(client_fd, (struct sockaddr *)&address,
                     (socklen_t*)&addrlen))<0)
  {
      perror("accept");
      exit(EXIT_FAILURE);
  }
  while(1) {
    sys_data *data = (sys_data*) malloc(sizeof(sys_data));
    data->cpu_load = GetCPULoad();
    send(new_socket , data , sizeof(sys_data), 0 );

#if DEBUG
    printf("CPU Load : %.2f%%\n", data->cpu_load);
#endif /* DEBUG */

    free(data);
  }
  return 0;
}
