#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc , char *argv[])
{
    ofstream outfile;
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];

    
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
        return 1;
    }
    puts("Socket created");

    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8881 );

    
    int opt = 1;
    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
       
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    
    listen(socket_desc , 3);

    
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

   
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
      
        char message[100] = "Message received: ";
        strcat(message, client_message);
        write(client_sock , message , strlen(message));

       
        outfile.open("pqr.txt", ios::app);
        outfile << client_message;
        outfile.close();
    }
    
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
    
    return 0;
}

