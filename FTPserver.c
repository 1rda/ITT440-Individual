#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SIZE 1024

void write_file(int socket_desc){
	int i;
	FILE *fp;
	char *filename = "adri2.txt";
	char buffer[SIZE];

	fp = fopen(filename, "w");
        if(fp == NULL){
                perror("Error in creating file.");
                exit(1);
        }

	while(1){
		i = recv(socket_desc, buffer, SIZE, 0);
		if(i <= 0){
			break;
			return;
		}
		fprintf(fp, "%s", buffer);
		bzero(buffer, SIZE);
	}

	return;
}

int main(int argc, char *argv[]){
	int socket_desc, new_socket, c;
	struct sockaddr_in server, client;
	char *message;

	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc < 0){
		printf("Could not create socket");
		return 1;
	}

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);

	//Bind
	if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
		puts("Bind failed");
		return 1;
	}
	puts("Bind done");

	//Listen
	listen(socket_desc, 3);

	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if(new_socket < 0){
                perror("accept failed");
                return 1;
        }
	puts("Connection accepted");

	write_file(new_socket);
	printf("Data has been written in the text file.\n");

	return 0;
}
