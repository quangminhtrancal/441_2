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
#include<sys/wait.h>

#define MAX_BUF_LEN 512
#define PORTRANGE_MIN 8000
#define MAX_NOUNS 5
#define MAX_ADJS 8
#define MAX_VERBS 6
#define MAX_DUMDUMS 3
#define MAX_LENGTH 10
#define TIME_OUT 10
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

		int quit = 0;
while (!quit)
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
//	int parentsockfd;
//	 int pid;
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

    
	if ((readBytes=recvfrom(s, buf, MAX_BUF_LEN, 0, client, &len))==-1)
						{
							printf("Read error!\n");
							quit = 1;
						}
	buf[readBytes] = '\0'; // padding with end of string symbol
	printf(" Server received: %s\n",buf);


	//while (!quit)
    //  {

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
					char sequence[9][100]={"10000000","01000000","00100000","00010000"
											,"00001000","00000100","00000010","00000001","11111111"};
					// Array to set the bit to 1 if the ACK of that sequence has been received for double checking
					int ACK_receive[9][1];
					memset(&ACK_receive, 0, sizeof(ACK_receive)); // Set to zero meaning that there is no receiving of any ACK yet
					int count_ACK=0; // Variable to check the number of ACK received from Client

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
									char temp[10000];
									strcpy(temp,sequence[count_leg]);
									strcat(temp,sub_buffer[count_leg]);
									strcpy(sub_buffer[count_leg],temp);
									count_leg+=1;
									index[count_leg]=index[count_leg-1]+size8;
								}
							}
						}
					}
					for (int i=0; i<8; i++){
						printf("Buffer %d, index=%d  is %s\n",i,index[i],sub_buffer[i]);
					}	
					//printf("Replaced buffer %s\n",replaced_buffer);
					printf("FILE buffer %s\n",file_buffer);
					strcpy(replaced_buffer,file_buffer);
					// if there is remaining
					if (mod8!=0){
						for (int j=0;j<mod8;j++){
							sub_buffer[8][j]=file_buffer[index[7]+j];
							int dex=index[7]+j;
							printf("J=%d , index=%d Before buf 8 %s %s\n",j,index[7],sub_buffer[8],&file_buffer[dex]);
						}
						
						int j=0;						
						for (j=mod8;j<size8;j++){
							sub_buffer[8][j]=' ';
						}
						sub_buffer[8][size8]='\0';
						printf("Before buf 8 %s\n",sub_buffer[8]);
						char temp[10000];
						strcpy(temp,sequence[8]);
						strcat(temp,sub_buffer[8]);
						strcpy(sub_buffer[8],temp);		
						count_ACK=9;
					}
					else count_ACK=8;

					printf("Buffer 8 is %s\n",sub_buffer[8]);
			int send=0; // Variable to check if all has been sent
					/// Follow pseudocode from assignment help
			while(send==0){
				//for (int i=0; i<9;i++){
				int send_index=0;
				while (send_index<9){
					if(sendto(s,sub_buffer[send_index],strlen(sub_buffer[send_index]),0, client, len)<0){
						printf("error in sending the leg\n");
						exit(1);
					}
					//***********************************Create a pipe1 for timer
					int fd1[2];  // Used to store two ends of first pipe; 0 for read and 1 for writing
					if (pipe(fd1)==-1)
					{
						fprintf(stderr, "Pipe 1 Failed" );
						return 1;
					}

					//Call fork() to create child1 process
					
					pid_t pid1;

					pid1=fork();
					if (pid1 < 0) {
						perror("UDP Server: ERROR while forking new process 1.\n");
						exit(1);
					}
					// check if the process ID is zero
					else if (pid1 == 0) {
						// we are now inside the child process for the 
						// Set timer
						char notification[100];
						printf("Timer is ON\n");
						sprintf(notification,"Start timer");
						write(fd1[1], notification, strlen(notification)+1);
						sleep(TIME_OUT);

						read(fd1[0], notification, 100);
						if (strncmp(notification,"SENT",4)==0){
							close(fd1[0]);	
							close(fd1[1]);
							exit(0);
						}
						else{

         					close(fd1[0]);  // Close reading end of first pipe
						 	sprintf(notification,"TIMEOUT");
				        	write(fd1[1], notification, strlen(notification)+1);
							printf("TIMER GOES OFF!!!!\n");
				        	close(fd1[1]);
							exit(0);
						}
					}
					else{
						// This is parent process

						// ********************************Create a pipe 2 for ACK
						
						int fd2[2];
						if (pipe(fd2)==-1)
						{
							fprintf(stderr, "Pipe 2 Failed" );
							return 1;
						}

						pid_t pid2=fork();
						if (pid2 < 0) {
							perror("UDP Server: ERROR while forking new process 2.\n");
							exit(1);
						}
						// check if the process ID is zero, this is child process
						else if (pid2 == 0) {
							if ((readBytes=recvfrom(s, buf, MAX_BUF_LEN, 0, client, &len))==-1)
							{
								printf("Read error!\n");
								quit = 1;
							}
							buf[readBytes] = '\0'; // padding with end of string symbol
							printf(" Child process received: %s\n",buf);

							if (strncmp(buf, "ACK", 3) == 0){
								write(fd2[1], buf, strlen(buf)+1);
								close(fd2[1]);
								close(fd2[0]); // close the reading								
								exit(0);
							} 
							else if (strncmp(buf, "OVER", 4) == 0){
								close(fd2[1]);
								close(fd2[0]); // close the reading								
								exit(0);
							}

						}
						else{
							int check=0;
							while (check==0){
								printf("This is parent process\n"); 
								char message1[100];
								read(fd1[0], message1, 100);
								printf("REcieve from timer: %s\n",message1);
								if (strncmp(message1,"TIMEOUT",7)==0){
									close(fd1[0]);
									close(fd1[1]);
									char temp[10]="OVER";
									write(fd2[1], temp, strlen(temp)+1);
									close(fd2[0]);
									close(fd2[1]);
									break;
								}
							//while(count_ACK!=0){
								char message2[100];
								char temp[10];
								read(fd2[0], message2, 100);
								if (strlen(message2)>0) check=1;
								printf("Receive ACK=%s\n",message2);
								bzero(temp,sizeof(temp));	
								strncpy(temp,message2+3,strlen(message2));
								int num=atoi(temp);
								close(fd2[0]);
								close(fd2[1]);
								
								if (num<=9 && num >=0){
										printf("Count ACK %d\n",count_ACK);							
										count_ACK-=1;
										ACK_receive[num][0]=1;
										sprintf(temp,"SENT");
										write(fd1[1], temp, strlen(temp)+1);
										send_index+=1;
										close(fd1[0]);
										close(fd1[1]);
								}
								if (count_ACK==0) {
									check=1;
									printf("int count_ACK=0\n");
									break;
								}
							}
							if (count_ACK==0) {
								send=1;
								kill(pid1,SIGKILL);
								kill(pid2,SIGKILL);
								printf(" killing subsequent\n");
								break;
							}
						}
					}
				}
			}
			printf("SEND DONE\n");
					// Reset to zero
			for (int i=0; i<9;i++){
				bzero(sub_buffer[i],sizeof(sub_buffer[i]));
				bzero(file_buffer,sizeof(file_buffer));	
			}
    close(s);
	//sleep(15);
    
  	}
  	return 0;		
  }

