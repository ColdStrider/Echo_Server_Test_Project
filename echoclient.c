#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <getopt.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>



/* Be prepared accept a response of this length */
#define BUFSIZE 512

#define USAGE                                                                       \
    "usage:\n"                                                                      \
    "  echoclient [options]\n"                                                      \
    "options:\n"                                                                    \
    "  -m                  Message to send to server (Default: \"Hello Spring!!\")\n" \
    "  -s                  Server (Default: localhost)\n"                           \
    "  -p                  Port (Default: 48384)\n"                                  \
    "  -h                  Show this help message\n"

/* OPTIONS DESCRIPTOR ====================================================== */
static struct option gLongOptions[] = {
    {"message", required_argument, NULL, 'm'},
    {"port", required_argument, NULL, 'p'},
    {"server", required_argument, NULL, 's'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0}};

/* Main ========================================================= */
int main(int argc, char **argv)
{
    int option_char = 0;
    char *message = "Hello Fall!!";
    char *hostname = "localhost";
    unsigned short portno = 48384;
    char buffer[BUFSIZE];

    signal(SIGPIPE, SIG_IGN);  // Handle SIGPIPE to avoid abrupt exit on broken pipe


    // Parse and set command line arguments
    while ((option_char = getopt_long(argc, argv, "s:p:m:hx", gLongOptions, NULL)) != -1) {
        switch (option_char) {
        case 'p': // listen-port
            portno = atoi(optarg);
            break;
        case 's': // server
            hostname = optarg;
            break;
        case 'm': // message
            message = optarg;
            break;
        case 'h': // help
            fprintf(stdout, "%s", USAGE);
            exit(0);
            break;
        default:
            fprintf(stderr, "%s", USAGE);
            exit(1);
        }
    }

    setbuf(stdout, NULL); // disable buffering

    if ((portno < 1025) || (portno > 65535)) {
        fprintf(stderr, "%s @ %d: invalid port number (%d)\n", __FILE__, __LINE__, portno);
        exit(1);
    }

    if (NULL == message) {
        fprintf(stderr, "%s @ %d: invalid message\n", __FILE__, __LINE__);
        exit(1);
    }

    if (NULL == hostname) {
        fprintf(stderr, "%s @ %d: invalid host name\n", __FILE__, __LINE__);
        exit(1);
    }
    //
    //
    //
    //
    /* Socket Code Here */
    

    //Declairs variables including the socket strucure for the server address
    int clientSocket;
    struct sockaddr_in serverAddress;

    //creates the socket 
    (clientSocket = socket(AF_INET, SOCK_STREAM, 0));

    //changes aspects of the stuct of the server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(portno);


    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    while (1)
    {
        //takes an inpute from user
        fgets(buffer, BUFSIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
            perror("Send failed");
            close(clientSocket);
            exit(EXIT_FAILURE);
        }

        int bytes_received = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            perror("Receive failed");
        } 
        
        /*else {
            buffer[bytes_received] = '\0'; 
            printf("Echo from server: %s\n", buffer);
        }
        */
        printf("%s", buffer);
        //write(STDOUT_FILENO, buffer, bytes_received);
    }
    
    
}
