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

  char* message_client;
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

   printf("Connected to the server at port %d\n", PORT);

    while (1) {
        printf("Enter the client's message (type 'exit' to quit): ");
        fgets(message_client, sizeof(buffer), stdin);
        message_client[strcspn(message_client, "\n")] = '\0'; // Removing newline character from fgets

        if (strcmp(message_client, "exit") == 0) {
            printf("Client is exiting...\n");
            break;
        }

        send(client_fd, message_client, strlen(message_client), 0);

        valread = read(client_fd, buffer, 0);
        buffer[valread] = '\0'; // Null-terminate the received buffer before printing
        printf("Server: %s\n", buffer);
    }

    // Close the socket
    close(client_fd);  
  return 0;

}
