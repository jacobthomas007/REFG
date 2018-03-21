#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<time.h>

void errormessage(char *);
struct ref{
char buffer[100];
int status;
time_t start;
time_t end;
time_t time;
int flag;
}door;
int s_bind;
int s_accept;
int s_listen;
void *doorStatus();
void *doorTime();
int main(int argc, char *argv[])
{
	int sock;
	unsigned int c_len;
	struct sockaddr_in s_sock;
	struct sockaddr_in c_sock;
	char pass[5];
	int size_recv;
	pthread_t thread1;
	pthread_t thread2;
	int tid1;
	int tid2;
//	if(argc < 2){
//	printf("Enter the port number <SERVER port>\n");
//	exit(0);
//	}
	unsigned short serverPORT = 8000;
	s_sock.sin_family = AF_INET;
	s_sock.sin_addr.s_addr = INADDR_ANY;
	s_sock.sin_port = htons(serverPORT);
	memset(door.buffer,0,sizeof(door.buffer));
	memset(pass,0,sizeof(pass));
	strcpy(pass,"123");
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock < 0)
		errormessage("socket creation failed");
	s_bind = bind(sock, (struct sockaddr *)&s_sock, sizeof(s_sock));
	if(s_bind < 0)
		errormessage("bind failed");
	s_listen = listen(sock, 3);
	if(s_listen < 0)
		errormessage("listen failed");
	printf("Server initialization ready, waiting for password \n");

		s_accept = accept(sock,(struct sockaddr *)&c_sock, &c_len);
		if(s_accept < 0)
			errormessage("accept failed");
		printf("Connected... verifing password...\n");
		size_recv = recv(s_accept,door.buffer,sizeof(door.buffer),0);
		if(size_recv < 0 )
		errormessage("recv failed");
		#if PACKET_DEBUG
		printf("DATA = %s  \n %d\n", door.buffer,(int)strlen(door.buffer));
		#endif
		if(strcmp(door.buffer,pass) == 0 )
		printf("Password success..\n");
		else
		errormessage("Password failed.. connection closed");
		#if PACKET_DEBUG	
		printf("DATA = %s  \n %d\n", door.buffer,(int)strlen(door.buffer));
		#endif
		memset(door.buffer,0,sizeof(door.buffer));
		//recv(s_accept,door.,sizeof(buffer),0);
		//printf("DATA = %s  \n %d\n", door.,(int)strlen(buffer));
		tid1 = pthread_create(&thread1,NULL,doorStatus,NULL);
		if(tid1 != 0)
		{
			printf("Cant create thread1\n");
			return 0;
		}
		tid2 = pthread_create(&thread2,NULL,doorTime,NULL);
		if(tid2 != 0)
		{
			printf("Cant create thread2\n");
			return 0;
		}
		pthread_join(thread1,NULL);
		pthread_join(thread2,NULL);
	return 0;
}

void *doorStatus()
{
	while(1)
	{
	recv(s_accept,door.buffer,sizeof(door.buffer),0);
	printf("DATA = %s  \n %d\n", door.buffer,(int)strlen(door.buffer));
	if(strcmp(door.buffer,"open") == 0)
	{
		printf("door is open\n");
		door.flag = 1; //open indication
		memset(door.buffer,0,sizeof(door.buffer));
	}
	if(strcmp(door.buffer,"close") == 0)
	{
		printf("door is close\n");
		door.flag = 0;
		memset(door.buffer,0,sizeof(door.buffer));
	}
	memset(door.buffer,0,sizeof(door.buffer));
	}
}
void *doorTime()
{
while(1)
{
	if(door.flag == 1)
	{
		//start the time claculation.
		door.flag = 10;// so nxt time when the thread exc does not enter here
		door.start = time(NULL);
	}
	else if(door.flag == 0)
	{
		door.flag = 10;
		door.end = time(NULL);
		printf("time the door was open is %ld\n",door.end - door.start);
	}
	else
	{
	}
}
}
void errormessage(char *message)
{
	perror(message);
	exit(1);
}
