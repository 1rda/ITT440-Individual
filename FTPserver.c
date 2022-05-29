#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SIZE 1024

//create a new file and write to it
void write_file(int socket_desc){
	int i;
	FILE *fp;
	char *filename = "adri2.txt";//file to be created
	char buffer[SIZE];

	fp = fopen(filename, "w");
        if(fp == NULL){
                perror("[!]Error in creating file.");
                exit(1);
        }

	//Write to the file
	while(1){
		i = recv(socket_desc, buffer, SIZE, 0);
		if(i <= 0){
			break;
			return;
		}
		fprintf(fp, "%s", buffer);
		bzero(buffer, SIZE);
	}
	printf("[+]File successfully received\n");

	return;
}

int main(int argc, char *argv[]){
	int socket_desc, new_socket, c;
	struct sockaddr_in server, client;
	char *message;

	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc < 0){
		printf("[!]Could not create socket");
		return 1;
	}

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);

	//Bind
	if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
		puts("[!]Bind failed");
		return 1;
	}

	//Listen
	listen(socket_desc, 3);

	puts("[+]Listening on port 8888...");
	c = sizeof(struct sockaddr_in);
	new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if(new_socket < 0){
                perror("[!]Connection failed");
                return 1;
        }
	puts("[+]Connection accepted");

	write_file(new_socket);

	close(socket_desc);
	puts("[+]Connection closed");

	return 0;
}
