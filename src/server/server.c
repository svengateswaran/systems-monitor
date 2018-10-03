/******************************************************************************
 *
 * Simple Remote Systems Monitoring Tool
 *
 * Author : Vengateswaran <saivengatraghu@gmail.com>
 *

 * ***************************************************************************/
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <definitions.h>
#include <sys/stat.h>
#include <pthread.h>
#include <assert.h>

void get_data_from_client(void *ip) {

  struct sockaddr_in address; 
  int sock = 0; 
  struct sockaddr_in client_addr; 
  char client_ip[16];
  strcpy(client_ip, (char*)ip);

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
  { 
      printf("\n Socket creation error \n"); 
      return -1; 
  } 
 
  memset(&client_addr, '0', sizeof(client_addr)); 
 
  client_addr.sin_family = AF_INET; 
  client_addr.sin_port = htons(PORT); 
     
  /* Convert IPv4 and IPv6 addresses from text to binary form */
  if (inet_pton(AF_INET, client_ip, &client_addr.sin_addr) <= 0)  
  { 
      printf("\nInvalid address/ Address not supported \n"); 
      return -1; 
  } 
 
  if (connect(sock, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) 
  { 
      printf("\nConnection Failed \n"); 
      return -1; 
  } 

  char data_dir[1024];
  sprintf(data_dir, "%s/%s", DATA_DIR, client_ip);
  mkdir(data_dir, 0777);
 
  char data_file[1024];
  sprintf(data_file, "%s/cpu_load", data_dir);
  
  while(1) {
    sys_data *data = (sys_data*) malloc(sizeof(sys_data));
    int valread = read(sock , data, sizeof(sys_data));
    FILE *data_fp = fopen(data_file, "w");
    fprintf(data_fp, "%.2f", data->cpu_load);
    fclose(data_fp);
    free(data);
  }
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("Usage : ./server <client-ip-addr>");
    return -1;
  }

  mkdir(DATA_DIR, 0777);

  pthread_t t;
  int pthread_ret_val = 0;
  int i;
  
  for(i = 1; i < argc; i++) {
    pthread_ret_val = pthread_create(&t, NULL, &get_data_from_client, (void *)(argv[i]));
    assert(pthread_ret_val == 0 && "Cannot create thread");
  }

  for(i = 1; i < argc; i++) {
    pthread_ret_val = pthread_join(t, NULL);
    assert(pthread_ret_val == 0 && "Cannot join thread");
  }

  return 0;
}
