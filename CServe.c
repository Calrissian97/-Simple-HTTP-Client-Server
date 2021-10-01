/*
Basic HTTP/1.1 web server C program using Berkeley (Linux) sockets
Landon Hull
5/6/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Network includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Desired port to operate on (HTTP/1 is conventionally 80)
#define PORT 80

/*
	Reads a file to be served as a char [] and appends it to the given header
	Params: Header- char [] containing the initial HTTP header
*/
void Create_HTTP_Message(char Header[])
{
    // Open desired file for reading
    FILE *WebPage = fopen("index.html", "r");

    char line[256];
    char responseData[8192];
	
	// Read file line by line and append it to responseData
    while (fgets(line, 256, WebPage) != 0) 
    {
        strcat(responseData, line);
    }
	
	// Append our given HTTP header with our desired message
    strcat(Header, responseData);
}

int main(void)
{
	// Char array holding the entire HTTP/1.1 message, with the header provided at initialization
    char http_message[8192] = "HTTP/1.1 200 OK\r\n\r\n";
	// Call function to append the desired message to the char array
    Create_HTTP_Message(http_message);

    // Create server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Define server address and details
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;			// IPv4
    server_address.sin_port = htons(PORT);			// Operating Port
    server_address.sin_addr.s_addr = INADDR_ANY;	// Accept any address

    // Bind the socket to our address and port
    int bound = bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    if (bound < 0)
        {
            printf("\n Bind Error");
            return 1;
        }

    // Set the socket to listen for clients (up to 2 pending connections for this example)
    int listening = listen(server_socket, 2);
    if (listening < 0)
    {
        printf("\n Listen Error");
        return 1;
    }

	// Primary serving loop
    while (1)
    {
        // Char array of client info
        char client[64];
		
		// Define client address
        struct sockaddr_in client_address;
        int client_address_size;

        // Create client socket
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_size);

        // Recieve IPv4 address of client
        inet_ntop(AF_INET, &client_address, client, 256);
		
		// Let admin know client has connected
        printf(" Client %s connected!\n", client);

        // Send message to client
        send(client_socket, http_message, sizeof(http_message), 0);

        // Close client socket and disconnect
        close(client_socket);
        printf(" Client %s Disconnected!\n", client);
    }
	
	// Close server socket and end program
    close(server_socket);
	return 0;
}