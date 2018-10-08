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

void write_data_i(char *dir, char *filename, int value, int prefix) {
    char full_file_path[FILE_PATH_LEN];
    sprintf(full_file_path, "%s/%s", dir, filename);
    if(prefix >= 0) sprintf(full_file_path, "%s_%d", full_file_path, prefix);
    FILE *fp = fopen(full_file_path, "w");
    fprintf(fp, "%d", value);
    DEBUG_INFO("%s : %d\n", filename, value);
    fclose(fp);
}
void write_data_f(char *dir, char *filename, float value, int prefix) {
    char full_file_path[FILE_PATH_LEN];
    sprintf(full_file_path, "%s/%s", dir, filename);
    if(prefix >= 0) sprintf(full_file_path, "%s_%d", full_file_path, prefix);
    FILE *fp = fopen(full_file_path, "w");
    fprintf(fp, "%.2f", value);
    DEBUG_INFO("%s : %.2f\n", filename, value);
    fclose(fp);
}
void write_data_ui(char *dir, char *filename, unsigned int value, int prefix) {
    char full_file_path[FILE_PATH_LEN];
    sprintf(full_file_path, "%s/%s", dir, filename);
    if(prefix >= 0) sprintf(full_file_path, "%s_%d", full_file_path, prefix);
    FILE *fp = fopen(full_file_path, "w");
    fprintf(fp, "%u", value);
    DEBUG_INFO("%s : %u\n", filename, value);
    fclose(fp);
}
void write_data_s(char *dir, char *filename, char* value, int prefix) {
    char full_file_path[FILE_PATH_LEN];
    sprintf(full_file_path, "%s/%s", dir, filename);
    if(prefix >= 0) sprintf(full_file_path, "%s_%d", full_file_path, prefix);
    FILE *fp = fopen(full_file_path, "w");
    fprintf(fp, "%s", value);
    DEBUG_INFO("%s : %s\n", filename, value);
    fclose(fp);
}
void write_data_ull(char *dir, char *filename, unsigned long long value, int prefix) {
    char full_file_path[FILE_PATH_LEN];
    sprintf(full_file_path, "%s/%s", dir, filename);
    if(prefix >= 0) sprintf(full_file_path, "%s_%d", full_file_path, prefix);
    FILE *fp = fopen(full_file_path, "w");
    fprintf(fp, "%llu", value);
    DEBUG_INFO("%s : %llu\n", filename, value);
    fclose(fp);
}

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
    sleep(1);
    DEBUG_INFO("%s : connecting ...\n", client_ip);
  } 

  char data_dir[1024];
  sprintf(data_dir, "%s/%s", DATA_DIR, client_ip);
  mkdir(data_dir, 0777);
 
  int gpu_index;

  while(1) {
    sys_data *data = (sys_data*) malloc(sizeof(sys_data));
    int valread = read(sock , data, sizeof(sys_data));
    if (valread != sizeof(sys_data)) {
       DEBUG_INFO("%s : read error! trying to reconnect ...\n", client_ip);
       close(sock);
       free(data);
       goto socket;
    }
    DEBUG_INFO("-----%s--------\n", client_ip);
    write_data_f(data_dir, "cpu_util", data->cpu_util, -1);
    write_data_f(data_dir, "disk_capacity", data->disk_capacity, -1);
    write_data_f(data_dir, "disk_freespace", data->disk_freespace, -1);
    write_data_f(data_dir, "disk_util", (data->disk_capacity - data->disk_freespace) * 100 / data->disk_capacity, -1);
    write_data_ui(data_dir, "gpu_count", data->gpu_count, -1);
    for (gpu_index = 0; data->gpu_count > 0 &&
                        data->gpu_count <= MAX_NUM_GPU &&
                        gpu_index < data->gpu_count; gpu_index++) {
      write_data_s (data_dir, "gpu_name",        data->gpu_name[gpu_index],              gpu_index);
      write_data_ui(data_dir, "gpu_cores_util",  data->gpu_cores_util[gpu_index],        gpu_index);
      write_data_ui(data_dir, "gpu_mem_util",    data->gpu_mem_util[gpu_index],          gpu_index);
      write_data_f (data_dir, "gpu_mem_total",   GET_GB(data->gpu_mem_total[gpu_index]), gpu_index);
      write_data_f (data_dir, "gpu_mem_used",    GET_GB(data->gpu_mem_used[gpu_index]),  gpu_index);
      write_data_ui(data_dir, "gpu_temperature", data->gpu_temperature[gpu_index],       gpu_index);
    DEBUG_INFO("-----%s-------x\n", client_ip);
    }
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
  char *clients_list = (char*) malloc(num_clients * CLIENT_LEN);

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
