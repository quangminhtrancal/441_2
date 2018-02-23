/* Simple program to demonstrate a basic UDP client.
 * Reads commands from stdin and sends them to the server.
 * Requests random nouns, adjectives, and verbs as you wish.
 * A "quit" message from the client kills the server gracefully.
 *
 * Compile using "gcc -o wordclient wordclient.c"
 */
 // sequence number, acknowledgement, time out, retransmission
 // name of file, how big is it?
// word server
// 736 = 8*92
// 739=


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>

#define MAX_BUF_LEN 1000000

/* Hardcode the IP address of the server (local or remote) */
/* #define SERVER_IP "127.0.0.1"   /* loopback interface */
//#define SERVER_IP "136.159.5.22"  /* rsx1.cpsc.ucalgary.ca */
#define SERVER_IP "127.0.0.1"  /* rsx1.cpsc.ucalgary.ca */

/* Edit as needed to match port of server */
#define SERVER_PORT 8001

int main(void)
  {

    // TCP -------------------- for getting file information
    /* Address initialization */
	struct sockaddr_in server1;
	int MYPORTNUM = 8002;
	memset(&server1, 0, sizeof(server1));
	server1.sin_family = AF_INET;
	server1.sin_port = htons(MYPORTNUM);
	server1.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Create the listening socket */
	int sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		printf("Error in socket() while creating lstn_sock\n");
		exit(-1);

	}

	/* Connect to TCP server */
	int status;
	status = connect(sock, (struct sockaddr *) &server1,
			sizeof(struct sockaddr_in));
	if (status < 0) {
		printf("Error in connect()\n");
		exit(-1);

	} else {
		printf("Connected.\n");
	}

	int c1=0;
	/* ASk client to send file command to get file information*/
			char message[1024];
	while(c1==0){
		  printf("Enter FILE to know information of files in server\n");
	
			scanf("%s", message);
			if(strncmp(message, "FILE", 4) == 0) c1=1;
	}

			int count = send(sock, message, sizeof(message), 0);
			if (count < 0) {
				printf("Error in send()\n");
				exit(-1);
			}
			/* Receive data */
			char rcv_message[1024];
			count = recv(sock, rcv_message, sizeof(rcv_message), 0);
			if (count < 0) {
				printf("Error in recv()\n");
				exit(-1);
			} else {
					printf("Server has below files: %s\n", rcv_message);
			}
  int check=0;
	char rawname[1024];
	char filename[1000];
	int filesize=0;
	char temp[1000];
	strcpy(rawname,rcv_message);
  	/* ASk client to send the file name*/
  while (check==0){
			memset(&message, 0, sizeof(message));
	    printf("Enter FILE name\n");
			scanf("%s", message);      
      count = send(sock, message, sizeof(message), 0);
      if (count < 0) {
        printf("Error in send()\n");
				exit(-1);
      }


			memset(&rcv_message, 0, sizeof(rcv_message));
      count = recv(sock, rcv_message, sizeof(rcv_message), 0);
      if (count < 0) {
        printf("Error in recv()\n");
				exit(-1);
      } else {
        printf("Server send this message: %s \n", rcv_message);
				if (strncmp(rcv_message, "DONE", 4) == 0)
					{
						strcpy(filename,message);
					  for (char* t = strtok(rawname, "\r\n"); t != NULL; t = strtok(NULL, "\r\n")){
									if (strstr(t,filename)) {
											printf("The data %s\n", t);
											int count=0;

											for (int i=0; i< strlen(t);i++){
												if (t[i]=='-'){
													for (int j=i+1;j<strlen(t);j++){
														temp[j-i-1]=t[j];
													}
													break;
												}
											}
											break;
										}
									}
						check=1;
					}
      }
  }
	filesize=atoi(temp);
	printf("The size is %d\n",filesize);


	/* Close the socket */
	close(sock);
    //-------------UDP ------------------------

    struct sockaddr_in si_server;
    struct sockaddr *server;
    int s, i, len = sizeof(si_server);
    char buf[MAX_BUF_LEN];
	//	int filesize=736;
    int readBytes;
    int quit;

	//	strcpy(filename,message); // Copy file name


    memset((char *) &si_server, 0, sizeof(si_server));
    si_server.sin_family = AF_INET;

    server = (struct sockaddr *) &si_server;

    if (inet_pton(AF_INET, SERVER_IP, &si_server.sin_addr)==0)
      {
					printf("inet_pton() failed\n");
					return 1;
      }
		si_server.sin_port = htons(SERVER_PORT);
		
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
      {
	printf("Could not setup a socket!\n");
	return 1;
      }

    quit = 0;
		char sequence[9][100]={"10000000","01000000","00100000","00010000"
											,"00001000","00000100","00000010","00000001","11111111"};
		char sub_buffer[9][10000];
		char total_receive[100000];
		memset(&sub_buffer, 0, sizeof(sub_buffer));
		memset(&total_receive, 0, sizeof(total_receive));
		check=0;
		int fsize=0;
		char ack[1000];
    while(!quit)
      {
					printf("Enter a command: ");
					scanf("%s", buf);

					if(strncmp(buf, "quit", 4) == 0)
						quit = 1;

					if (sendto(s, buf, strlen(buf), 0, server, sizeof(si_server)) == -1)
						{
							printf("sendto failed\n");
							return 1;
						}

					if ((readBytes=recvfrom(s, buf, MAX_BUF_LEN, 0, server, &len))==-1)
						{
							printf("Read error!\n");
							quit = 1;
						}
					//buf[readBytes] = '\0'; // padding with end of string symbol
					// Check if the receive is the octo-leg
					if (strlen(buf)==(filesize/8+8)){
							char order[100];
							strncpy(order,buf,8);
							for (int i=0; i<9;i++){
								if (sequence[i]==order){
									strcpy(sub_buffer[i],buf);
									sprintf(ack,"ACK%d",i);
									if (sendto(s, ack, strlen(ack), 0, server, sizeof(si_server)) == -1)
									{
										printf("sendto failed\n");
										return 1;
									}
									memset(&ack, 0, sizeof(ack));
									break;
								}
							}
							fsize+=strlen(buf)-8;
					}

					if (fsize==filesize){
						//merge all sequence
							for (int i=0; i<9;i++){
								strcat(total_receive,sub_buffer[i]);
							}
						FILE *f;
						remove("test.txt");
						f = fopen("test.txt", "a");
						size_t file_size=ftell(f);
						if (file_size < filesize) {
							
							fprintf(f, "%s", total_receive);
						}
						fclose(f);

						printf("From server: \"%s\"\n\n", total_receive);
     			}
			}


    close(s);
    return 0;
  }
