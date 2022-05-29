#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#define SIZE 1024

//Send the .txt file
void send_file(FILE *fp, int socket_desc){
	char data[SIZE] = {0};

	while(fgets(data, SIZE, fp) != NULL){
		if(send(socket_desc, data, sizeof(data), 0) < 0){
			perror("[!]Error in sending data");
			exit(1);
		}
		bzero(data, SIZE);
	}
}

int main(int argc, char *argv[]){
	int socket_desc;
	struct sockaddr_in server;
	char *message;
	FILE *fp;
	char *filename = "adri.txt"; //file to be sent

	//Create Socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc == -1){
		printf("[!]Could not create socket");
		return 1;
	}

	server.sin_addr.s_addr = inet_addr("192.168.56.101"); //server address
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	//Connect to FTP server
	if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
		puts("[!]Connection failed");
		return 1;
	}
	puts("[+]Connected");

	//Open file
	fp = fopen(filename, "r");
	if(fp == NULL){
		perror("[!]Error in reading file.");
		return 1;
	}

	send_file(fp, socket_desc);
	printf("[+]File sent successfully\n");

	close(socket_desc);
	printf("[+]Connection closed\n");

	return 0;
}
