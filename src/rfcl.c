#include<stdio.h>/* printf(), perror()*/
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>/*socket() bind() connect()*/
#include<arpa/inet.h>/*sockaddr_in */
#include<unistd.h>/*close()*/

void errormessage(char *);/*display error messages and exit*/
int main(int argc, char *argv[])
{

	int sock;/*socket descriptor*/
	int c_connect;
	struct sockaddr_in c_sock;
	char *serverIP;
	unsigned short serverPORT;
	char buff[100];
	if ((argc < 3) || (argc > 3))	
	{
		fprintf(stderr,"Execute format <serverIP> <PORt>\n");
		exit(1);
	}
	//serverIP = argv[1];
	serverPORT = atoi(argv[2]);
	//printf("IP = %s\n", serverIP);
	//printf("PORT = %d\n", serverPORT);
	/*
	   socket(int protocol_family,int type,int protocol)
	   socket() --> connect() --> send/recv() --> close()
	   sockaddr_in
	   {
	   unsigned short sin_family; //2bytes AF_INET;
	   unsigned shoty sin_port;  // 2bytes port address to which the particular 
	   socket communicates
	   struct in_addr sin_addr; //4bytes (e.g 192.168.1.1)
	   char unused[n]; // unused fields
	   }
	 */
	c_sock.sin_family = AF_INET;
	c_sock.sin_port = htons(serverPORT); //server port
	c_sock.sin_addr.s_addr = inet_addr(argv[1]);//server IP

	sock = socket(PF_INET,SOCK_STREAM,0);
	if(sock < 0)
		errormessage("socket creation failed");
	/*connect(int socket, struct sockaddr_in *address,unsigned int addresslen)*/
	c_connect = connect(sock, (struct sockaddr *)&c_sock, sizeof(c_sock));
	if(c_connect  < 0)
		errormessage("connect failed");
	printf("Enter the pasword to contiune \n");
	scanf("%s",buff);
	//strcpy(buff,"Hello world");
	send(sock, buff, strlen(buff), 0);
	memset(buff,0,sizeof(buff));
	printf("Enter the table number\n");
	while(1){
	scanf("%s",buff);
	send(sock, buff, strlen(buff), 0);
	}
	return 0;
}
void errormessage(char *message)
{
	perror(message);
	exit(1);
}

