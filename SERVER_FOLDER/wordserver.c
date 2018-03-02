/* Server to transfer file
 * written by Minh from the original source of wrodserver.c
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
#define HUGE_NUMBER 500000
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

					int count = recv(connected_sock, rcv_message, sizeof(rcv_message), 0);
					printf("Received message is %s\n",rcv_message);
					if (count < 0) {
						printf("Error in recv()\n");
					}
					else {
						if(strncmp(rcv_message, "FILE", 4) == 0)
						{

							//	char filename[1024] = { "\n736.txt-736\r\n739.txt-739\r\n1KB.txt-1024\r\n2KB.txt-2048\r\n4KB.txt-4096\r\n8KB.txt-8192\r\n8888.txt-8888\r\n32KB.txt-32768\r\n256KB.txt-262144\r\n" };

								char filename[1024] = { "\n736.txt-736\r\n739.txt-739\r\n1KB.txt-1024\r\n2KB.txt-2048\r\n4KB.txt-4096\r\n8KB.txt-8192\r\n8888.txt-8888\r\n32KB.txt-32768\r\n256KB.txt-262144\r\n" };

								count = send(connected_sock, filename, sizeof(filename), 0);
								if (count < 0) {
									printf("Error in send()\n");
								}
						}
				}
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




					// OPEN FILE and READ AND SEND

						char file_buffer[1000000];
                        char file_buffer1[1000000];
                        char file_buffer2[1000000];
                        char file_buffer3[1000000]; // for last part 6640
						char replaced_buffer[1000000];

						// Size processing
					int size8=0;
					int mod8=0;
					int index[8];
                    int check_large=0;
					char sub_buffer[8][10000];
					memset(&replaced_buffer, 0, sizeof(replaced_buffer));

					memset(&sub_buffer, 0, sizeof(sub_buffer));

						// Send information
					char c;
					FILE *fp;
					fp = fopen(filename, "r");

					int index1 = 0;
					while (fscanf(fp, "%c", &c) != EOF) {
							file_buffer[index1] = c;
							index1++;
					}
						file_buffer[index1] = '\0';
						int file_size = index1;
                          printf("File %s size is %d\n",filename,file_size);
						fclose(fp);

                    // Split into two smaller char array for transfering the biggest file
                    int half=file_size/2;
                  if(strcmp(filename,"256KB.txt")==0){
                      strncpy(file_buffer1,file_buffer,half);
                      strncpy(file_buffer2,file_buffer+half,file_size-6640);
                      strncpy(file_buffer3,file_buffer+255504,file_size);
                      check_large=4;
                      printf("buf1 %lu buf2 %lu buf3 %lu\n",strlen(file_buffer1),strlen(file_buffer2),strlen(file_buffer3) );
                  }
                  else{
                      check_large=1;
                  }

					// Break into smaller chunks
      while(check_large>0){



          int remaining_size=0;
          if(check_large==1){
              strcpy(replaced_buffer,file_buffer);
              remaining_size=file_size;
              check_large=0;
          }
          else if(check_large==4){
              printf("Sending first part of the biggest file\n %s\n",file_buffer1);
              strcpy(replaced_buffer,file_buffer1);
              remaining_size=half;
              check_large=3;
          }
          else if(check_large==3){
              printf("Sending second part of the biggest file\n %s\n",file_buffer1);
              strcpy(replaced_buffer,file_buffer2);
              remaining_size=half-6640;
              check_large=2;
          }
          else if(check_large==2){
            printf("Sending last part of the biggest file\n %s\n",file_buffer2);
              strcpy(replaced_buffer,file_buffer3);
              remaining_size=6640;
              check_large=0;
          }
            printf("Start the party started %d\n",check_large);

					//printf("FILE CONTENT %s\n", file_buffer);
					printf("--------------------------------------\n");
					char sequence[8][100]={"10000000","01000000","00100000","00010000"
											,"00001000","00000100","00000010","00000001"};
					// Array to set the bit to 1 if the ACK of that sequence has been received for double checking
					int ACK_receive[8][1];
					memset(&ACK_receive, 0, sizeof(ACK_receive)); // Set to zero meaning that there is no receiving of any ACK yet
					int count_ACK=0; // Variable to check the number of ACK received from Client

					int start=0;
  while(remaining_size>0)
		{
					memset(&sub_buffer, 0, sizeof(sub_buffer));
					int p=0;
					//using index and sub_buffer to send
					//if (remaining_size > 8888) {
					if (remaining_size > 8) {
						// divide to smaller octoputs
						//memset(&replaced_buffer, 0, sizeof(replaced_buffer));
						//strcpy(replaced_buffer,file_buffer);
						if (remaining_size>8888){
							int size1=8888;
							size8=size1/8;
						}
						else{
							size8=remaining_size/8;
							char check_buf[100000];
							strncpy(check_buf,replaced_buffer+start,strlen(replaced_buffer));
							//printf("----- The current remaining is--------: \n%s\n",check_buf);
							p=1;
						}
						memset(&sub_buffer, 0, sizeof(sub_buffer));

						int count_leg=0;
						index[0]=start+size8;
						while(count_leg<8){
							//for (int j=start;j<strlen(replaced_buffer);j++){
								//sub_buffer[count_leg][j%size8]=replaced_buffer[j]; // Get each element in the buffer to put in the sub-buffer
								//if(j==index[count_leg]-1){
									if (count_leg==0) strncpy(sub_buffer[count_leg],replaced_buffer+start,size8);
									else strncpy(sub_buffer[count_leg],replaced_buffer+index[count_leg-1],size8);
									char temp[10000];
									//sub_buffer[count_leg][j+1]='\0';
									//if(p==1) printf("%s------------\n",sub_buffer[count_leg]);
									strcpy(temp,sequence[count_leg]);
									strcat(temp,sub_buffer[count_leg]);
									strcpy(sub_buffer[count_leg],temp);
									//sub_buffer[count_leg][j+9]='\0';
									count_leg+=1;
									index[count_leg]=index[count_leg-1]+size8;
								//}
							//}
						}
						for (int i=0; i<8; i++){
//							printf("For larger Buffer %d, index=%d  is \n%s\n",i,index[i],sub_buffer[i]);
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
//-						printf("Buffer %d, index=%d  is %s\n",i,index[i],sub_buffer[i]);
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
//-						printf("I am here in ACK child\n");

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

//-							printf(" Child process received: %s\n",buf);

						//remove(note);
						f = fopen(note, "wba");
						fprintf(f, "%s", buf);
						fclose(f);

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
									// Read it all in one operation
										read_size = fread(buff, 1, sizeof(buff), file);
//-									printf("Buffer in timer before %s \n",buff);
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
//-								printf("This is parent process %d\n",send_index);
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
									// Read it all in one operation
										read_size = fread(buff, 1, sizeof(buff), file);
//-									printf("Buffer in father %s \n",buff);
								}
								fclose(file);

								if(strncmp(buff, "ACK", 3) == 0)
								{
									check=1;
//-									printf("Parent receive ACK=%s\n",buff);
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
          printf("PArt finish check=%d\n",check_large);

  }// For the bigger file size
			printf("Length of the buffer %d\n",strlen(file_buffer));
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
