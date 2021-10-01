/*
Basic HTTP/1.1 web client C program using Berkeley (Linux) sockets
Landon Hull
5/6/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Network includes
#include <sys/types.h>

// Desired port to operate on (HTTP/1 is conventionally 80)
#define PORT 80

int main(int argc, char* argv[])
{
	// Argument must be given to connect, argument should be a valid IPv4 address
    if (argc > 1)
    {
        // Create socket of Internet family, Stream, TCP protocol
        int server_socket = socket(AF_INET, SOCK_STREAM, 0);
		
		// Given address to connect to
		char* server_addr = argv[1];

        // Define server address details
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET; // Family of address
        server_address.sin_port = htons(PORT); // Port in endian format
        inet_pton(AF_INET, server_addr, &server_address.sin_addr); // Address to connect to from given value

        // Connect to socket with sockaddr_in server_address cast to sockaddr
        int connected = connect(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
        if (connected < 0)
        {
            printf("\n Connection Error");
            return 1;
        }
        
        // Recieve up to 8192 bytes of data from server
        char HTML_response[8192];
        recv(server_socket, &HTML_response, sizeof(HTML_response), 0);

        printf("\n Server says: %s\n", HTML_response);

        // Close the socket
        close(server_socket);
    }
	return 0;
}