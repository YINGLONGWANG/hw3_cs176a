#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
char * get_message(int seq_num);


int main(int argc, char * argv[]) {
	struct sockaddr_in server_address, return_address;
	
	char server_ip[] = "127.0.0.1";
	int port = 12000;
	int i = 0;
	int socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr( server_ip );
 	server_address.sin_port = htons(port);
	
	while (i<10){
		const char * message = get_message(i);
	
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
			printf("error\n");

		//if result < 0, then timeout occurred
		//else, teimout did not occur
		//set sockopt ensures that result returns less than 0 if a timeout is reached.
		

		printf("Received '%s'\n", message);
	i++;
	}
	close(socket_id);
}

char * get_time(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
 	long double millisecondsSinceEpoch =
		(long double)(tv.tv_sec) * 1000 +
 		(long double)(tv.tv_usec) / 1000;
 	char* time = malloc(100);
        sprintf(time, "%Lf", millisecondsSinceEpoch);

	return time;
}

char * get_message(int seq_num){
	char * time = get_time();
	char * message= malloc(400);
	char * number = malloc(4);
	sprintf(number, "%d", seq_num);
	strcpy(message, "PING ");
	strcat(message, number);
	strcat(message, " ");
	strcat(message, time);
	

	return message;
}
