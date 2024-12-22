#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#define BUFSIZE 1024
#define PINGS 10
#define TIMEDOUT_SEC 1

int main(int argc, char *argv[]) //client is a program that takes is called with two command-line arguments
	//if number of arguments does not equal 3, print usage statement and exit program
{	if (argc != 3) {
		fprintf(stderr, "usage: %s <hostname> <port_num>\n", argv[0]);
		exit(1);
	}
    //retrieves char input which is the hostname from command-line
	const char *hostname = argv[1];
    //retrieves integer argument 2 which is the port number from command-line
	int port_num= atoi(argv[2]);
    //creates server_addr object in struct sockaddr_in
	struct sockaddr_in server_addr;
	char buf[BUFSIZE];
	int c_sock;
	//initialize number of packets sent and received to 0
	int num_sent=0;
	int num_received=0;
	///type socklen_t for variable addr_len  which is initialized with the size of server address
	socklen_t addr_len=sizeof(server_addr);
	//initialize min rtt to 1000.0 ms and initialize the max and total to 0.0 ms
	double rtt_min=1000.0;
	double rtt_max=0.0;
	double rtt_total=0.0;

	//create UDP socket
	c_sock=socket(AF_INET, SOCK_DGRAM, 0);
	//if socket is less than 0 print error message and exit program
	if (c_sock<0) {
		perror("Can not create socket");
		exit(1);
	}

	//zeroes out memory for server_addr
	bzero(&server_addr, sizeof(server_addr));
	//program runs on the INET domain
	server_addr.sin_family = AF_INET;
	//sets provided port number for the server to listen on
	server_addr.sin_port = htons(port_num);
	//converts the string hostname into binary IP address
	inet_pton(AF_INET, hostname, &server_addr.sin_addr);

	struct timeval timevalue={
		.tv_sec =TIMEDOUT_SEC,
		.tv_usec = 0
	};
	
	setsockopt(c_sock, SOL_SOCKET, SO_RCVTIMEO, &timevalue, sizeof(timevalue));
    
	//sends 10 pings
	for (int i = 0; i < PINGS; i++) {
		struct timeval begin;
		struct timeval end;
        //get time of day for the beginning time value
		gettimeofday(&begin, NULL);
		
		if (sendto(c_sock, "PING", strlen("PING"), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
			perror("Send failed");
			continue;
		}

        //output message that PING was sent
		printf("PING %d: Sent... ", i + 1);
        //increment num_sent by 1
		num_sent++;
		//if a response is received from the server, if it is recieved store the response in buf
		if (recvfrom(c_sock, buf, BUFSIZE, 0, (struct sockaddr *)&server_addr, &addr_len) > 0) {
			//get time of day for the end time value
			gettimeofday(&end, NULL);
			//calculate rtt from seconds to milliseconds and from microseconds to milliseconds for rtt
			double rtt = (end.tv_sec - begin.tv_sec) * 1000.0 + (end.tv_usec - begin.tv_usec) / 1000.0;
			printf("RTT=%f ms\n", rtt);
            //add rtt to total_rtt
			rtt_total += rtt;
            //if rtt is less than rtt_min, rtt_min=rtt
			if (rtt < rtt_min) {
				rtt_min = rtt;
			}
            //if rtt is more than rtt_max, rtt_max=rtt
			if (rtt > rtt_max) {
				rtt_max = rtt;
			}
            //increment num_received by 1
			num_received++;
		}
		else {//else the program timed out
			printf("Timed out\n");

		}
	}
    //calculate loss rate with num_sent and num_recieved
	double loss_rate = ((num_sent - num_received) / (double)num_sent) * 100;
	//calculate average rtt if num_recieved is more than 0 rtt_avg = total rtt/ recieved
	
	double rtt_avg; 
	//if num_received is more than 0 rtt_avg = total rtt/ received
	if(num_received > 0){
		rtt_avg = rtt_total/num_received;
	}
	//else average rtt=0;
	else{
		rtt_avg=0;
	}
 
   	 //print packet and rtt info
	printf("\n%d pkts xmited, %d pkts rcvd, %.0f%% pkt loss\n", num_sent, num_received, loss_rate);
	printf("min: %f, max: %f, avg: %f\n", rtt_min, rtt_max, rtt_avg);
    //close c_sock
	close(c_sock);
	return 0;
}

