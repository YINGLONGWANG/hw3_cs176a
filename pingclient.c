#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
char * get_message(int seq_num, long double millisecondsSinceEpoch);
long double get_time();

int main(int argc, char * argv[]) {
	struct sockaddr_in server_address, return_address;
	
	char server_ip[] = "127.0.0.1";
	int port = 12000;
	int i = 1;
	int socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr( server_ip );
 	server_address.sin_port = htons(port);
	
	while (i<11){
		long double send_time = get_time(); 
		const char * message = get_message(i, send_time);
	
		printf("Sending '%s' to %s:%i\n", message, server_ip, port);
		sendto( socket_id, message, strlen(message), 0,
			(struct sockaddr *) &server_address, sizeof(struct sockaddr_in) );
		printf("Waiting for Response\n");
		
		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		setsockopt(socket_id, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv));
		unsigned int return_len;
		int result = recvfrom( socket_id, message, strlen(message), 0,
			(struct sockaddr *) &return_address, &return_len );
		if (result<0)
			printf("Request timeout for icmp_seq %d\n", i);
		else
		{
			//want to print:
			//PING received form machine_name: seq# = X time = Y
			//not sure if I can store the return time. would def
			//be easier
			/*int space_number = 0;
			for (int j = 0; j<stren(message); j++)
			{
				if (message[i] = " ")
					space_number++;
				if(space_number
				
			}*/
		}

		//if result < 0, then timeout occurred
		//else, teimout did not occur
		//set sockopt ensures that result returns less than 0 if a timeout is reached.
		

		printf("Received '%s'\n", message);
	i++;
	}
	close(socket_id);
}

long double get_time(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
 	long double millisecondsSinceEpoch =
		(long double)(tv.tv_sec) * 1000 +
 		(long double)(tv.tv_usec) / 1000;
 	return millisecondsSinceEpoch;
}

char * get_message(int seq_num, long double millisecondsSinceEpoch){
	char* time = malloc(100);
	sprintf(time, "%Lf", millisecondsSinceEpoch);
	
	char * message= malloc(400);
	char * number = malloc(4);
	sprintf(number, "%d", seq_num);
	strcpy(message, "PING ");
	strcat(message, number);
	strcat(message, " ");
	strcat(message, time);
	

	return message;
}
