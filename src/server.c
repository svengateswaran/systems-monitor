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

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("Usage : ./server <client-ip-addr>");
    return -1;
  }

  struct sockaddr_in address; 
  int sock = 0, valread; 
  struct sockaddr_in client_addr; 
  char *hello = "Hello from server"; 
  char buffer[1024] = {0}; 
  char client_ip[16];

  strcpy(client_ip, argv[1]);

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
  send(sock , hello , strlen(hello) , 0 ); 
  printf("Hello message sent\n"); 
  valread = read( sock , buffer, 1024); 
  printf("%s\n",buffer );

  return 0;
}
