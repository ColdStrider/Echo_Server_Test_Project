#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <getopt.h>
#include <sys/socket.h>
#include <stdio.h>

#define BUFSIZE 512

#define USAGE                                                        \
    "usage:\n"                                                         \
    "  echoserver [options]\n"                                         \
    "options:\n"                                                       \
    "  -m                  Maximum pending connections (default: 5)\n" \
    "  -p                  Port (Default: 48384)\n"                    \
    "  -h                  Show this help message\n"

/* OPTIONS DESCRIPTOR ====================================================== */
static struct option gLongOptions[] = {
    {"port",          required_argument,      NULL,           'p'},
    {"maxnpending",   required_argument,      NULL,           'm'},
    {"help",          no_argument,            NULL,           'h'},
    {NULL,            0,                      NULL,             0}
};


int main(int argc, char **argv) {
    int portno = 48384; /* port to listen on */
    int option_char;
    int maxnpending = 5;
    char buffer[BUFSIZE];

  
    // Parse and set command line arguments
    while ((option_char = getopt_long(argc, argv, "p:m:hx", gLongOptions, NULL)) != -1) {
        switch (option_char) {
        case 'm': // server
            maxnpending = atoi(optarg);
            break; 
        case 'p': // listen-port
            portno = atoi(optarg);
            break;                                        
        case 'h': // help
            fprintf(stdout, "%s ", USAGE);
            exit(0);
            break;
        default:
            fprintf(stderr, "%s ", USAGE);
            exit(1);
        }
    }

    setbuf(stdout, NULL); // disable buffering

    if ((portno < 1025) || (portno > 65535)) {
        fprintf(stderr, "%s @ %d: invalid port number (%d)\n", __FILE__, __LINE__, portno);
        exit(1);
    }
    if (maxnpending < 1) {
        fprintf(stderr, "%s @ %d: invalid pending count (%d)\n", __FILE__, __LINE__, maxnpending);
        exit(1);
    }


  /* Socket Code Here */
    struct sockaddr_in server_addr, client_addr;
    int serverSocket, new_socket;

    (serverSocket = socket(AF_INET, SOCK_STREAM, 0));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(portno);

    //Binds the socket 
    bind(serverSocket,(struct sockaddr * ) &server_addr, sizeof(server_addr));

    //Listen for connection from the client
    listen(serverSocket, 5);

    //printf("listening for port %d", portno);


    socklen_t client_addr_len = sizeof(client_addr);

    //accepting the connection continuously 
    while (1)
    {
        (new_socket = accept(serverSocket, (struct sockaddr*)&server_addr, &client_addr_len));

        while (1)
        {
            memset(buffer, 0, sizeof(buffer));
            
            //receiving data
            int bytes_received = recv(new_socket, buffer, sizeof(buffer), 0);

            /*if (bytes_received ==0){
                printf("Client closed connection\n");
                close(new_socket);
                break;
            }*/
            
            if (bytes_received <= 0) {
                perror("Receive failed");
                close(new_socket);
                break;
            }

            if (send(new_socket, buffer, bytes_received, 0) == -1) {
                perror("Send failed");
                close(new_socket);
                break;
            }

            //printf("Echoed: %s\n", buffer);
        }
        
        

        // Close client socket
        //close(new_socket);
    }
    printf("test1");
}
