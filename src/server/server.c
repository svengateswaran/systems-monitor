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


void* get_data_from_client(void *ip) {

  struct sockaddr_in address; 
  int sock = 0; 
  struct sockaddr_in client_addr; 
  char client_ip[16];
  strcpy(client_ip, (char*)ip);
  
  socket:
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
  { 
      printf("\n Socket creation error \n"); 
      return NULL; 
  } 
 
  memset(&client_addr, '0', sizeof(client_addr)); 
 
  client_addr.sin_family = AF_INET; 
  client_addr.sin_port = htons(PORT); 
     
  /* Convert IPv4 and IPv6 addresses from text to binary form */
  if (inet_pton(AF_INET, client_ip, &client_addr.sin_addr) <= 0)  
  { 
      printf("\nInvalid address/ Address not supported - %s\n", client_ip); 
      return NULL; 
  } 
 
  while (connect(sock, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
    DEBUG_INFO("%s : connecting ...\n", client_ip);
  } 

  char data_dir[1024];
  sprintf(data_dir, "%s/%s", DATA_DIR, client_ip);
  mkdir(data_dir, 0777);
 
  char data_file[1024];
  sprintf(data_file, "%s/cpu_load", data_dir);
  
  while(1) {
    sys_data *data = (sys_data*) malloc(sizeof(sys_data));
    int valread = read(sock , data, sizeof(sys_data));
    if (valread < 1) {
       DEBUG_INFO("%s : read error! trying to reconnect ...\n", client_ip);
       close(sock);
       free(data);
       goto socket;
    }
    FILE *data_fp = fopen(data_file, "w");
    fprintf(data_fp, "%.2f", data->cpu_load);
    fclose(data_fp);
    free(data);
  }
}

int file_exists(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL)
    return 0;
  fclose(fp);
  return 1;
}

int read_words(char *clients_list) {
  FILE *client_fp = fopen(CLIENTS_LIST, "r");
  char client[CLIENT_LEN];
  int num_clients = 0;
  while (!feof(client_fp)) {
    int read = fscanf(client_fp, "%s", client);
    if(read > 0 && client[0] != '\n') {
      num_clients++;
      DEBUG_INFO("client : |%s|\n", client);
    }
  }
  fclose(client_fp);
  return num_clients;
}

int main() {

  /* read clients' list */
  if (!file_exists(CLIENTS_LIST)) {
    printf("Clients' list not found - %s\nPlease create and add clients to the file\n", CLIENTS_LIST);
    return -1;
  }
  int num_clients = read_words(CLIENTS_LIST);
  char *clients_list = malloc(num_clients * CLIENT_LEN);

  FILE *client_fp = fopen(CLIENTS_LIST, "r");
  int client_i = 0;
  while (!feof(client_fp)) {
    fscanf(client_fp, "%s", clients_list + (client_i * CLIENT_LEN));
    client_i++;
  }
  fclose(client_fp);

  mkdir(DATA_DIR, 0777);

  pthread_t t;
  int pthread_ret_val = 0;
  int i;
  
  for(i = 0; i < num_clients; i++) {
    DEBUG_INFO("Connecting .. %s\n", (clients_list + (i * CLIENT_LEN)));
    pthread_ret_val = pthread_create(&t, NULL, &get_data_from_client, (void *)(clients_list + (i * CLIENT_LEN)));
    assert(pthread_ret_val == 0 && "Cannot create thread");
  }

  for(i = 0; i < num_clients; i++) {
    pthread_ret_val = pthread_join(t, NULL);
    assert(pthread_ret_val == 0 && "Cannot join thread");
  }

  return 0;
}
