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
#define TIME_OUT 1
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
//while (!quit)
//{
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
					char sub_buffer[8][10000];
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
					int remaining_size=file_size;
					int start=0;
		while(remaining_size>0)
		{
					memset(&sub_buffer, 0, sizeof(sub_buffer));
					//using index and sub_buffer to send
					if (remaining_size > 8888) {
						// divide to smaller octoputs
						int size1=8888;
						size8=size1/8;
						int count_leg=0;
						index[0]=start+size8;
						while(count_leg<8){
							for (int j=start;j<strlen(replaced_buffer);j++){
								sub_buffer[count_leg][j%size1]=replaced_buffer[j]; // Get each element in the buffer to put in the sub-buffer
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
						for (int i=0; i<8; i++){
							printf("For larger Buffer %d, index=%d  is %s\n",i,index[i],sub_buffer[i]);
						}
						remaining_size=remaining_size-size1;
						start=index[7];
					}
					else if (remaining_size <= 8888 && remaining_size >8){
						memset(&replaced_buffer, 0, sizeof(replaced_buffer));
						strcpy(replaced_buffer,file_buffer);

						size8=remaining_size/8;
					
						int count_leg=0;
					
						index[0]=start+size8;
						while(count_leg<8){
							for (int j=0;j<strlen(replaced_buffer);j++){
								sub_buffer[count_leg][j%size8]=replaced_buffer[j]; // Get each element in the buffer to put in the sub-buffer
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
						for (int i=0; i<8; i++){
						printf("Buffer %d, index=%d  is %s\n",i,index[i],sub_buffer[i]);
						}
						remaining_size=remaining_size-size8*8;
						start=index[7];

					}
					else{  // remaining size <8

						printf("LESS THAN 8 %d\n",remaining_size);
					//printf("Replaced buffer %s\n",replaced_buffer);
					//printf("FILE buffer %s\n",file_buffer);
						memset(&replaced_buffer, 0, sizeof(replaced_buffer));
						strcpy(replaced_buffer,file_buffer);
					// if there is remaining
							for (int j=0;j<remaining_size;j++){
								sub_buffer[j][0]=replaced_buffer[start+j];
								char temp[10000];
								strcpy(temp,sequence[j]);
								strcat(temp,sub_buffer[j]);
								strcpy(sub_buffer[j],temp);
							}
							int j=0;						
							for (j=remaining_size;j<8;j++){
								sub_buffer[j][0]=' ';
								char temp[10000];
								strcpy(temp,sequence[j]);
								strcat(temp,sub_buffer[j]);
								strcpy(sub_buffer[j],temp);
							}

									for (int i=0; i<8; i++){
						printf("Buffer %d, index=%d  is %s\n",i,index[i],sub_buffer[i]);
						}	
						remaining_size=0;
					}
					count_ACK=8;
			/////// ----------------------------- SENDING ONE BY ONE LEG----------
				int num=-1;
				int send_index=0;
				int check=0;
				while (send_index<8){
					if(sendto(s,sub_buffer[send_index],strlen(sub_buffer[send_index]),0, client, len)<0){
						printf("error in sending the leg\n");
						exit(1);
					}

					//***********************************Create a pipe1 for ACK
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
					// check the ACK
					else if (pid1 == 0) {
						close(fd1[0]);
						//sleep(1);
						printf("I am here in ACK child\n");

							char notification[100]=" ";
							char note[1024]="Notification.txt";
							FILE *f;
							//remove(note);
							f = fopen(note, "wba");
							fprintf(f, "%s", notification);
							fclose(f);
						
						if ((readBytes=recvfrom(s, buf, MAX_BUF_LEN, 0, client, &len))==-1)
							{
								printf("Read error!\n");
								quit = 1;
							}
						buf[readBytes] = '\0'; // padding with end of string symbol
						
							printf(" Child process received: %s\n",buf);
						
						//remove(note);
						f = fopen(note, "wba");
						fprintf(f, "%s", buf);
						fclose(f);
					
						/*
						if (readBytes>3){

							if (strncmp(buf, "ACK", 3) == 0){
								write(fd1[1], buf, strlen(buf));
								close(fd1[1]);
							} 							
						}
						else{
							printf("Child process, no ACK\n");
							char message[100]="NO_ACK";
							write(fd1[1], message, strlen(message));
							close(fd1[1]);
							
						}
						*/
								exit(0);
						
					}
					else{
						// This is parent process

						// ********************************Create a pipe 2 for timer
						
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
							close(fd2[0]);
							char notification[100];
							sleep(TIME_OUT);
							//read(fd2[0], notification, 1);
							
							int string_size=0;
							int read_size;	
							char buff[100];
							FILE *file;
							file = fopen("Notification.txt", "r");
							if (file) {
									// Seek the last byte of the file
									fseek(file, 0, SEEK_END);
									// Offset from the first to the last byte, or in other words, filesize
									string_size = ftell(file);
									// go back to the start of the file
									rewind(file);
									//printf("File size:\n %d \n",string_size);
									// Allocate a string that can hold it all
									
									//printf("size of allocated buffer: \n%d\n",sizeof(buff)); 
									// Read it all in one operation
										read_size = fread(buff, 1, sizeof(buff), file);
									printf("Buffer in timer before %s \n",buff);	
								}
							fclose(file);	

							if (read_size !=4){
								sprintf(notification,"TIMEOUT");
								char note[1024]="Notification.txt";
								FILE *f;
								f = fopen(note, "wba");
								fseek(f,0,SEEK_END);
								size_t file_size=ftell(f);
								//printf("Timer child file size before %d\n",file_size);
								fprintf(f, "%s", notification);
								printf("  TIMER GOES OFF!!!!OMG\n");
								fclose(f);
							}
							//write(fd2[1], notification, strlen(notification)+1);
							close(fd2[1]);
							
								
							exit(0);
						}
						// Parent process
						else{ 
							check=0;
							//int PACK=0;							
							while (check==0){
								printf("This is parent process %d\n",send_index); 
								sleep(1);

								// OPEN FILE and READ AND SEND
								int string_size=0;
								int read_size;	
								char buff[100];
								memset(&buff, 0, sizeof(buff));
								FILE *file;
								file = fopen("Notification.txt", "r");
								if (file) {
									// Seek the last byte of the file
									fseek(file, 0, SEEK_END);
									// Offset from the first to the last byte, or in other words, filesize
									string_size = ftell(file);
									// go back to the start of the file
									rewind(file);
									//printf("File size:\n %d \n",string_size);
									// Allocate a string that can hold it all
									
									//printf("size of allocated buffer: \n%d\n",sizeof(buff)); 
									// Read it all in one operation
										read_size = fread(buff, 1, sizeof(buff), file);
									printf("Buffer in father %s \n",buff);	
								}
								fclose(file);		
								
									//{
									//	printf("unable to copy file into buffer\n");
									//	exit(1);
									//}
								if(strncmp(buff, "ACK", 3) == 0)
								{
									check=1;
									printf("Parent receive ACK=%s\n",buff);
									char temp[10];
									bzero(temp,sizeof(temp));	
									strncpy(temp,buff+3,strlen(buff));
									num=atoi(temp);
									count_ACK-=1;
									send_index+=1;
									num=-1;
									break;
								}
								else if (strncmp(buff, "TIMEOUT", 7) == 0)
								{
									printf("Parent process timer goes off---------\n");
									kill(pid1,SIGKILL);
									kill(pid2,SIGKILL);
									printf("killing subsequent\n");
									break;										
								}
								fclose(fp);


		
							} // while check loop for checking ACK and timer
						} // End of parent process 2
					} // End of parent process 1
					printf("----------------\n");
				}// send index while loop
			//} // send
			printf("Current remaining size is %d\n",remaining_size);
	} // For confirming sending done  by using remaining size
			printf("SEND DONE\n");
					// Reset to zero
			for (int i=0; i<9;i++){
				bzero(sub_buffer[i],sizeof(sub_buffer[i]));
				bzero(file_buffer,sizeof(file_buffer));	
			}
    close(s);
	//sleep(15);
    
  //	} // main While loop
  	return 0;		
  }

