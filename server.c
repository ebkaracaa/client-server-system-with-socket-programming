#include <stdio.h>		// declarations used in most input and output
#include <sys/types.h>	// definations of data types used in syscalls. helps execute socket and netinet
#include <sys/socket.h> //If the socket function is successful, it creates a socket and
// returns a file descriptor for the socket to be used in subsequent calls. In case of
// failure, it returns to the -1 value. Defines the sockaddr structure.
#include <netinet/in.h> // constants and structures needed for internet domain adresses.eg: sockaddr_in
#include <stdlib.h>		// int atoid(const char *str) converting port number to integer
#include <unistd.h>
#include <string.h>
#include <netdb.h>

void socketController(int socketFileDescriptor); // function declarations
void inputController(int argc);
void bindingController(int bind_output);
void acceptController(int newSocketFileDescriptor);
void dataController(int data);

int main(int argc, char *argv[])
{ // int argc: total number of parameters we are passing
  // in our program we have 2 parameters: file name and port number.
  // if the user dont point the port number, user should be informed that they need to provide port number

	int socketFileDescriptor, newSocketFileDescriptor, portNumber, data;
	char temp_msg[255];
	struct sockaddr_in server_address, client_address; // sockaddr_in: contains internet adress
	socklen_t len_client;							   // socklen_t:32bit data  type

	inputController(argc);

	// We have to store the port number.
	portNumber = atoi(argv[1]); // argv[0]--> file name, argv[1]--> port number. Port number came from the CLI as string format, so atoi convert string to int.
	// CREATE SOCKET
	socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM: TCP, 0: default protocol as IP

	socketController(socketFileDescriptor);

	// SETTING PORT AND ADDRESS FOR SERVER
	server_address.sin_family = AF_INET;		 // TODO INET IPV4 İÇİN, AYNI MAKİNE OLDUĞU İÇİN AF_UNIX DENE
	server_address.sin_addr.s_addr = INADDR_ANY; // if you run this in same pc, you can specify the IP adress.
	server_address.sin_port = htons(portNumber);

	// BINDING THE SOCKET
	int bind_output;
	bind_output = bind(socketFileDescriptor, (struct sockaddr *)&server_address, sizeof(server_address));
	bindingController(bind_output);

	// LISTEN
	listen(socketFileDescriptor, 2); // request queue of size 2, one request at a time
	len_client = sizeof(client_address);

	// ACCEPT
	newSocketFileDescriptor = accept(socketFileDescriptor, (struct sockaddr *)&client_address, &len_client); // for every request by client, new socket will be created as newSocketFileDescriptor.
																											 // all information of the client will be stored in this structure. If you don't care about the address of the requesting socket, you can pass NULL and 0 for addr and addrlen.
	acceptController(newSocketFileDescriptor);

	while (1)
	{
		// bzero((char *)&server_address, sizeof(server_address));
		printf("Server: ");
		bzero(temp_msg, 255);
		data = read(newSocketFileDescriptor, temp_msg, 255); // get data from the client.
		dataController(data);

		// show client message
		printf("\nClient:%s", temp_msg);
		// to use same buffer, clear it.
		bzero(temp_msg, 255);

		fgets(temp_msg, 255, stdin); // getting server's data to buffer(temp_msg). fgets can use getting the data from the file as well. use stdin for read from the console
		data = write(newSocketFileDescriptor, temp_msg, 255);
		dataController(data);
		// to end this loop, last message should be start with "adios"
		int i = strncmp("adios", temp_msg, 5); // returns 0 if argument length are matched.
		if (i == 0)
		{
			printf("Adios!\n");
			break;
		}
	}

	close(socketFileDescriptor);
	return 0;
}

void socketController(int socketFileDescriptor)
{

	if (socketFileDescriptor < 0) // connection fail. // 36da tanımlamadım mıyav
	{
		printf("ERROR! SOCKET CANT OPEN.\n");
		exit(1);
	}
}

void inputController(int argc)
{
	if (argc < 2)
	{
		fprintf(stderr, "Wrong input! Enter as : filename.c + IP + portnumber. See u\n");
		exit(1);
	}
}

void bindingController(int bind_output)
{
	if (bind_output < 0)
	{
		printf(" ERROR IN BINDING!\n");
	}
	else
		(printf("Binding succesfull! Listening...\n"));
}
void acceptController(int newSocketFileDescriptor)
{
	if (newSocketFileDescriptor < 0)
	{
		printf("COULD NOT ACCEPT THE REQUEST!\n");
		exit(1);
	}
	else
		(printf("Request accepted!\n"));
}
void dataController(int data)
{
	if (data < 0)
	{
		printf("READING ERROR!\n");
		exit(1);
	}
}
// TODO SERVER KAPANDIKTAN SONRA CLIENT KAPANMIYOR
// TODO MESAJLAR TEK TEK GİDİP GELİYOR
//  BRUTAL PROJECT'I ÖNCEKI GİBİ YAPMAYA ÇALIŞ