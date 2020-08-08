#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <iostream>
#define PORT 8080

// Basic server code based on Ulrich 
int main() 
{ 

    int socket_handle = socket(AF_INET, SOCK_DGRAM, 0); // AF_INET = IPv4, SOCK_DGRAM for UDP
    if (socket_handle < 0)
    {
        perror("Unable to create socket!");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); // port 8080 as an example here
    server_addr.sin_addr.s_addr = INADDR_ANY; // own IP address
    struct sockaddr_in client_addr;
    socklen_t client_length = sizeof(client_addr);
    char* buffer = new char[1024]; // Store information received from client
    char client_ip[256]; // Store client IP, where the data came form

    bzero(&(server_addr.sin_zero), 8); // Make sure padding is only storing 0s

    if (bind(socket_handle, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) == SO_ERROR) // Bind address to socket
    {    
        perror("Unable to bind socket to port. Is the port in use?");
        return 1;
    }

    while (true)
    {
        int bytes_in = recvfrom(socket_handle, buffer, 1024, 0, (struct sockaddr *) &client_addr, &client_length); // Receive message from socket (sent from client), store it in buffer

        if (bytes_in = SO_ERROR)
        {
            std::cout << "Error recieving data from client!" << std::endl;
            continue;
        }

        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, 256); // Convert IPv4 address to string and store as client IP
        std::cout << "Message received from: " << client_ip << " : " << buffer << std::endl;
    }
} 
