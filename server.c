#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#define BUFSIZE 1024//the size of message buffer
int main(int argc, char *argv[])//server is a program that takes is called with one command-line argument
	//if number of arguments does not equal 2, print usage statement and exit program
{	if (argc != 2) { 
        fprintf(stderr, "usage: %s <port_num>\n", argv[0]); 
        exit(1); 
    }
    //retrieves input port number from command-line
    int port_num = atoi(argv[1]);
    //creates server_addr and client_addr objects in struct sockaddr_in
    struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	//UDP socket
	int u_sock; 
	//type socklen_t for variable client which is initialized with the size of client address
    	socklen_t client_len=sizeof(client_addr);     
	//buffer to store the messages                    
    	unsigned char buf[BUFSIZE];
	//the length of the message recieved from the client
	int recv_len;
	
    //create UDP socket
	u_sock= socket(AF_INET,SOCK_DGRAM, 0);
	//if socket is equal to -1 print error message and exit program
    if (u_sock == -1) {
        perror("opening datagram socket");
        exit(1);

    }
    
    //bind the socket
    //zeroes out memory for server_addr
    bzero(&server_addr, sizeof(server_addr));
    //program runs on the INET domain
    server_addr.sin_family = AF_INET; 
    //sets IP address for the server to accept a connection from any available network
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //sets provided port number for the server to listen on
    server_addr.sin_port = htons(port_num);
    
   //if bind returns -1, binding failed so print error and exit the program
    if (bind (u_sock, (struct sockaddr *)&server_addr, sizeof (server_addr)) == -1) {
        perror("binding datagram socket");
        close(u_sock);
        exit(1);
    }
	//seed random number generator with the current time
        srand(time(0));
    
	//infinite loop listening for UDP packets from client
    while(1){
	       
        printf("ready to recieve data...\n");
	
	//receives message from client
	//recv_len is the length of the message recieved from the client
        recv_len = recvfrom(u_sock, buf, BUFSIZE, 0, (struct sockaddr *)&client_addr, &client_len);
	//if the length of the message is less than 0, print error recvfrom failed and exit program
        if (recv_len < 0) {
            perror("recvfrom failed"); 
            continue; 
        } 
	
	//makes sure that the message buf can be printed as a string
        buf[recv_len] = '\0';
	//prints message from client
        printf("[client]: %s\n", buf);

        //simulates 30% packet loss
        if ((rand() % 100) < 30) { 
            printf("[server]: dropped packet\n"); 
            continue; 
        }
	 //Sends PONG message to client program
        const char *response = "PONG"; 
        
	//if 0 bytes are written sendto failed, print error sendto failed and exit program
        if (sendto(u_sock, response, strlen(response), 0, (struct sockaddr *)&client_addr, client_len) == -1) {
 	    	perror("sendto failed");

        }

    }

    return 0;

}
