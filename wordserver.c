/* Simple program to demonstrate a UDP-based server.
 * Binds to a random port between 8000 and 8999.
 * Receives a message from the client.
 * If it receives a "noun" message, it sends a random noun back.
 * If it receives a "adj" message, it sends a random adjective back.
 * If it receives a "verb" message, it sends a random verb back.
 * If you send an unknown command, it sends a random insult back.
 * A "quit" message from client kills the server gracefully.
 *
 * Compile using "gcc -o wordserver wordserver.c"
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <signal.h>
#include <netdb.h>
#include<fcntl.h>

#define MAX_BUF_LEN 512
#define PORTRANGE_MIN 8000
#define MAX_NOUNS 5
#define MAX_ADJS 8
#define MAX_VERBS 6
#define MAX_DUMDUMS 3
#define MAX_LENGTH 10

/* Global variable */
int childsockfd;

/* This is a signal handler to do graceful exit if needed */
void catcher( int sig )
  {
    close(childsockfd);
    exit(0);
  }

int main(int argc, char *argv[])
  {
		// ------------TCP to sdk for FILE--------------------------
/* Address initialization */
	struct sockaddr_in server1;
	int MYPORTNUM = 8002;
	memset(&server1, 0, sizeof(server1));
	server1.sin_family = AF_INET;
	server1.sin_port = htons(MYPORTNUM);
	server1.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Create the listening socket */
	int lstn_sock;
	lstn_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (lstn_sock < 0) {
		printf("Error in socket() while creating lstn_sock\n");
		exit(-1);
	}
	printf("Server is listening\n");
	/* Bind the socket to address and port */
	int status;
	status = bind(lstn_sock, (struct sockaddr *) &server1,
			sizeof(struct sockaddr_in));
	if (status < 0) {
		printf("Error in bind()\n");
		exit(-1);
	}
		/* Connect to TCP server */
	status = listen(lstn_sock, 5);
	if (status < 0) {
		printf("Error in listen()\n");
		exit(-1);
	}

		int rcv_file_request=0;


			/* Accept a connection */
					int connected_sock;
					connected_sock = accept(lstn_sock, NULL,NULL);
					if (connected_sock < 0) {
						printf("Error in accept()\n");
						exit(-1);
					}

			// check to receive FILE command
//			int f=0;
			char rcv_message[1024];
//			while (f==0){
					int count = recv(connected_sock, rcv_message, sizeof(rcv_message), 0);
					printf("Received message is %s\n",rcv_message);
					if (count < 0) {
						printf("Error in recv()\n");
					} 
					else {
						if(strncmp(rcv_message, "FILE", 4) == 0)
						{

								char filename[1024] = { "\n736.txt-736\r\n739.txt-739\r\n1KB.txt-1024\r\n2KB.txt-2048\r\n4KB.txt-4096\r\n8KB.txt-8192\r\n8888.txt-8888\r\n32KB.txt-32768\r\n256KB.txt-262144\r\n" };
								count = send(connected_sock, filename, sizeof(filename), 0);
								if (count < 0) {
									printf("Error in send()\n");
								}

/*								char message2[1024]={"Done1"};
								count = send(connected_sock, message2, sizeof(message2), 0);
								if (count < 0) {
									printf("Error in send()\n");
								}
								f=1;*/
						}
				}
	//		}

					/* Receive data */
		while (rcv_file_request == 0){

					memset(&rcv_message, 0, sizeof(rcv_message));
					int count = recv(connected_sock, rcv_message, sizeof(rcv_message), 0);
					printf("Received message is %s\n",rcv_message);
					if (count < 0) {
						printf("Error in recv()\n");
					} 
					else {

						if(strncmp(rcv_message, "736.txt", 7) == 0) {
							rcv_file_request=1;
							//printf("inside here\n");
						}
						else if(strncmp(rcv_message, "739.txt", 7) == 0) rcv_file_request=2;
						else if(strncmp(rcv_message, "1KB.txt", 7) == 0) rcv_file_request=3;
						else if(strncmp(rcv_message, "2KB.txt", 7) == 0) rcv_file_request=4;
						else if(strncmp(rcv_message, "4KB.txt", 7) == 0) rcv_file_request=5;
						else if(strncmp(rcv_message, "8KB.txt", 7) == 0) rcv_file_request=6;
						else if(strncmp(rcv_message, "8888.txt", 8) == 0) rcv_file_request=7;
						else if(strncmp(rcv_message, "32KB.txt", 8) == 0) rcv_file_request=8;
						else if(strncmp(rcv_message, "256KB.txt", 9) == 0) rcv_file_request=9;
						else {
								char message2[1024] = { "That file is not exsit, resend the right file name\n" };
								count = send(connected_sock, message2, sizeof(message2), 0);
								if (count < 0) {
									printf("Error in send()\n");
								}
						}
					}

			if (rcv_file_request>0) {
				


				char message1[1024] = {"DONE\n"};
				count = send(connected_sock, message1, sizeof(message1), 0);
					if (count < 0) {
						printf("Error in send()\n");
					}
			}
		}


					close(connected_sock);
			/* Close the socket */
		close(lstn_sock);

		//----------- UDP ----------------------
	int parentsockfd;
	 int pid;
	struct sockaddr_in si_server, si_client;
    struct sockaddr *server, *client;
    int s, i, len=sizeof(si_server);
    int port;					// Port number
    char buf[MAX_BUF_LEN];
    char tosend[10];
    int readBytes;
    int choice;
		char filename[1024];
		strcpy(filename, rcv_message); // SAVE file name

    port=8001;
    printf("Server listening on port %d\n", port);

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
      {
	printf("Could not setup a socket!\n");
	return 1;
      }

    memset((char *) &si_server, 0, sizeof(si_server));
    si_server.sin_family = AF_INET;
    si_server.sin_port = htons(port);
    si_server.sin_addr.s_addr = htonl(INADDR_ANY);
    server = (struct sockaddr *) &si_server;
    client = (struct sockaddr *) &si_client;
    if (bind(s, server, sizeof(si_server))==-1)
      {
	printf("Could not bind to port %d!\n", port);
	return 1;
      }

    int quit = 0;
    while (!quit)
      {

					if ((readBytes=recvfrom(s, buf, MAX_BUF_LEN, 0, client, &len))==-1)
						{
							printf("Read error!\n");
							quit = 1;
						}
					buf[readBytes] = '\0'; // padding with end of string symbol
					printf(" Server received: %s\n",buf);

					if(strncmp(buf, "quit", 4) == 0) quit = 1;

					if( quit == 1 ) sprintf(tosend, "%s", "OK");

					// OPEN FILE and READ AND SEND
					FILE *fp;
						char file_buffer[1000000];
						char replaced_buffer[1000000];

						fp=fopen(filename,"r");
						if(fp==NULL)
							{
								printf("file does not exist\n");
							}
						fseek(fp,0,SEEK_END);
						size_t file_size=ftell(fp);
					printf("The file size is %d\n", file_size);
						// Size processing
					int size8=0;
					int mod8=0;
					int index[8];
					char sub_buffer[9][10000];
					memset(&replaced_buffer, 0, sizeof(replaced_buffer));

					memset(&sub_buffer, 0, sizeof(sub_buffer));
				
						// Send information
					fseek(fp,0,SEEK_SET);
					if(fread(file_buffer,file_size,1,fp)<=0)
							{
								printf("unable to copy file into buffer\n");
								exit(1);
							}

					// Break into smaller chunks
					strcpy(replaced_buffer,file_buffer);

					//printf("Replace buffer %s\n", file_buffer);
					char sequence[8][100]={"000","001","010","011"
											,"100","101","110","111"};
					if (file_size > 8888) {
						// divide to smaller two octoput
					}
					else if (file_size <= 8888){
						size8=file_size/8;
						mod8=file_size % 8; 
						int count_leg=0;
					
						index[0]=size8;
						while(count_leg<8){
							for (int j=0;j<strlen(replaced_buffer);j++){
								sub_buffer[count_leg][j%size8]=replaced_buffer[j];
								if(j==index[count_leg]-1){
									sub_buffer[count_leg][j%size8+1]='$';
									sub_buffer[count_leg][j%size8+2]=sequence[count_leg][0];
									sub_buffer[count_leg][j%size8+3]=sequence[count_leg][1];
									sub_buffer[count_leg][j%size8+4]=sequence[count_leg][2];
									count_leg+=1;
									index[count_leg]=index[count_leg-1]+size8;
								}
							}
						}
					}
					for (int i=0; i<8; i++){
						printf("Buffer %d, index=%d  is %s\n",i,index[i],sub_buffer[i]);
					}	
					if (mod8!=0){
						for (int j=1;j<mod8;j++){
							sub_buffer[8][j]=replaced_buffer[index[7]+j];
						}
						for (int j=mod8;j<size8;j++){
							sub_buffer[8][j]=' ';
						}
					}
					printf("Buffer 8 is %s\n",sub_buffer[8]);
					// Send to client

						if(sendto(s,file_buffer,strlen(file_buffer),0, client, len)<0)    {
							printf("error in sending the file\n");
							exit(1);
						}
						bzero(file_buffer,sizeof(file_buffer));	
      }
    close(s);
    return 0;
  }

int max(int num1, int num2) {

   /* local variable declaration */
   int result;
 
   if (num1 > num2)
      result = num1;
   else
      result = num2;
 
   return result; 
}