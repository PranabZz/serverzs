#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc,char const* argv[]){
  
  int status, valread ,client_fd;

  struct sockaddr_in serv_addr;

  char* message_client = "Hello from client";
  char buffer[1024] = {0};

  if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
    printf("\n Scoket creation error ");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if(inet_pton(AF_INET, "127.0.0.1" , &serv_addr.sin_addr) <= 0){
    printf("\n Invalid address ");
    return -1;
  }

  if((status == connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) <0 ){
    printf("\n The connection failed");
    return -1;
  }

  while(status > 0){
    char* client;
    printf("Enter the clients message");
    scanf("%s",client);

    send(client_fd,client,strlen(client),0);

    printf("\n Sent message to the server \n");
    
    valread = read(client_fd,buffer,1024);

    printf("%s\n",buffer);

    close(client_fd);
  }
  return 0;

}
