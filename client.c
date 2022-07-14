#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>  // for inet_addr

// same port number should be entered with the server.

int main(int argc, char *argv[])
{
	int socketFileDescriptor, portNumber, data;
	struct sockaddr_in server_address;
	char temp_msg[255];

	portNumber = atoi(argv[2]); // argv[0]--> file name,argv[1]--> IP. argv[2]--> port number.
	socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFileDescriptor < 0) // connection fail.
	{
		printf("ERROR! SOCKET CANT OPEN.\n");
		exit(1);
	}

	// SETTING THE STRUCTURE
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(portNumber);
	server_address.sin_addr.s_addr = inet_addr(argv[1]); // some string format stuff, search inet_addr if you want.

	if (connect(socketFileDescriptor, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		printf("CONNECTION FAILED! TRY DIFFERENT PORT NUMBER.\n");
		exit(1);
	}
	else(printf("Connection succesfull!\n"));

	while (1)
	{
		printf("Client: ");
		bzero(temp_msg, 255);
		fgets(temp_msg, 255, stdin);
		data = write(socketFileDescriptor, temp_msg, strlen(temp_msg)); // client initializes the communicaiton.
		if (data < 0)
		{
			printf("READING ERROR!\n");
			exit(1);
		} // check if the writing is successful or not

		bzero(temp_msg, 255);

		data = read(socketFileDescriptor, temp_msg, 255); // read back from the server
														  // check if the reading is successful or not
		if (argc < 3)
		{
			printf("Wrong input! Enter as : filename.c + IP + port number.Adios!\n");
			exit(1);
		}
		// RESPONSE OF SERVER
		printf("Server: %s", temp_msg);

		// to end this loop, last message should be start with "adios"
		int i = strncmp("adios", temp_msg, 5);
		if (i == 0)
		{
			printf("Adios!\n");
			break;
		};
	}
	close(socketFileDescriptor);
	return 0;
}