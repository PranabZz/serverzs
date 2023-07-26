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
  
     printf("\nServer has started and is waiting for connections...\n");

    if ((new_socket = accept(socket_fd, (struct sockaddr*)&address, (socklen_t*)&addr_len)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is connected to a client on port %d\n", PORT);

    while (1) {
        
        char* server_message;

        printf("Enter the message you want to send (type 'exit' to quit): ");
        fgets(server_message,sizeof(buffer), stdin);

        server_message[strcspn(server_message, "\n")] = '\0'; // Removing newline character from fgets

        if (strcmp(server_message, "exit") == 0) {
            printf("Server is exiting...\n");
            break;
        }

        send(new_socket, server_message, strlen(server_message), 0);

        valread = read(new_socket, buffer, 0);
        buffer[valread] = '\0'; // Null-terminate the received buffer before printing
        printf("Client: %s\n", buffer);
    }

    // Close sockets
    close(new_socket);
    close(socket_fd);

  return 0;


}
