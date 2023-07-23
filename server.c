// All the headers required for the server

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080


int main(int argc, char const* argv[]){

  int socket_fd, new_socket, valread;

  struct sockaddr_in address; // Making an address struct for sockaddr_in 

  int opt = 1; // Used for optional ports if collied 

  int addr_len = sizeof(address);
  char buffer[1024] = {0}; // Buffer is used for sending data here we are using 1024 for 1mb
  
  char* message_server = "Connected to the server"; // The hello acts as a message which holds the value hello from the server
  
  // Noe the importaint part (Socket() create)
  // Creating socket ( domain = AF_LOCAL used for connection in local host AF_INET usese IPv4, SOCK_STREAM is the type TCP , 0 is the protocal ) 
  
  if((socket_fd = socket(AF_INET, SOCK_STREAM,0)) < 0){
    // If the server_fd is empty
    perror("Socket failed");
    exit(EXIT_FAILURE);
  }

  // Forcefully attachh socket to the the port 8080 so that we specify the port and the errors doesnot occur like same port using by two different service (Optional)
  
  if(setsockopt(socket_fd, SOL_SOCKET , SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt) )){
      perror("setsockeopt");
      exit(EXIT_FAILURE);
  } 

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY; // We use local host since we have INADDR_ANY
  address.sin_port = htons(PORT);
  
  // Binding
  
  if(bind(socket_fd, (struct sockaddr*)&address,sizeof(address)) < 0){
    perror("Bind was unsucessfull");
    exit(EXIT_FAILURE);
  }

  // Listen to the client

  if(listen(socket_fd,3)<0){
    perror("Listen was unsucessfull");
    exit(EXIT_FAILURE);
  }

  // Creating a new socket ( mirroring to send or recive data after accepting the connection request from the client)
  
  if((new_socket = accept(socket_fd, (struct sockaddr*)&address, (socklen_t*)&addr_len))<0){
    perror("Aceept failed");
    exit(EXIT_FAILURE);
  }

  valread = read(new_socket, buffer, 1024);
  printf("%s\n",buffer);
  
  send(new_socket,message_server,strlen(message_server), 0);

  printf("Server message sent");
 
  return 0;


}
