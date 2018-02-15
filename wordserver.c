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

#include <string.h>

#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>

#define MAX_BUF_LEN 512
#define PORTRANGE_MIN 8000
#define MAX_NOUNS 5
#define MAX_ADJS 8
#define MAX_VERBS 6
#define MAX_DUMDUMS 3
#define MAX_LENGTH 10

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
		while (rcv_file_request == 0){

										//--- WHILE loop
				/* Accept a connection */
					int connected_sock;
					connected_sock = accept(lstn_sock, NULL,NULL);
					if (connected_sock < 0) {
						printf("Error in accept()\n");
						exit(-1);
					}

					/* Send data*/
					int count;
					char message[1024] = { "For termination send \"Bye\"\n" };
					count = send(connected_sock, message, sizeof(message), 0);
					if (count < 0) {
						printf("Error in send()\n");
					}
					/* Receive data */

					char rcv_message[1024];
					count = recv(connected_sock, rcv_message, sizeof(rcv_message), 0);
					printf("Received message is %s\n",rcv_message);
					if (count < 0) {
						printf("Error in recv()\n");
					} else {
						if(strncmp(rcv_message, "FILE", 4) == 0)
						{
								char filename[10][1024] = { "736.txt-736","739.txt-739","1KB.txt-1024",
								"2KB.txt-2048","4KB.txt-4096","8KB.txt-8192","8888.txt-8888","32KB.txt-32768","256KB.txt-262144" };
								count = send(connected_sock, filename[1], sizeof(filename[1]), 0);
								if (count < 0) {
									printf("Error in send()\n");
								}
								char message1[1024] = {"Choose a file name from the list\n"};
								count = send(connected_sock, message1, sizeof(message1), 0);
								if (count < 0) {
									printf("Error in send()\n");
								}
						}
						else if(strncmp(rcv_message, "736.txt", 7) == 0) rcv_file_request=1;
						else if(strncmp(rcv_message, "739.txt", 7) == 0) rcv_file_request=2;
						else if(strncmp(rcv_message, "1KB.txt", 7) == 0) rcv_file_request=3;
						else if(strncmp(rcv_message, "2KB.txt", 7) == 0) rcv_file_request=4;
						else if(strncmp(rcv_message, "4KB.txt", 7) == 0) rcv_file_request=5;
						else if(strncmp(rcv_message, "8KB.txt", 7) == 0) rcv_file_request=6;
						else if(strncmp(rcv_message, "8888.txt", 8) == 0) rcv_file_request=7;
						else if(strncmp(rcv_message, "32KB.txt", 8) == 0) rcv_file_request=8;
						else if(strncmp(rcv_message, "256KB.txt", 9) == 0) rcv_file_request=9;
						else {
								char message2[1024] = { "That file is not exsit\n" };
								count = send(connected_sock, message2, sizeof(message2), 0);
								if (count < 0) {
									printf("Error in send()\n");
								}
						}
					}

			if (rcv_file_request>0) {
				close(connected_sock);
				char message1[1024] = {"Request receive\n"};
				count = send(connected_sock, message1, sizeof(message1), 0);
					if (count < 0) {
						printf("Error in send()\n");
					}
			}
		}


		
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
    char nounlist[MAX_NOUNS][MAX_LENGTH] = { "dog", "horse", "lamb", "pony", "skunk" };
    char adjlist[MAX_ADJS][MAX_LENGTH] = { "big", "fat", "huge", "lazy", "melodious", "pretty", "stinky", "yucky" };
    char verblist[MAX_VERBS][MAX_LENGTH] = { "ate", "endured", "jumped", "ran", "sat", "puked" };
    char dummylist[MAX_DUMDUMS][MAX_LENGTH] = { "dummy", "idiot", "moron" };

    printf("Generating random input port!\n");
    srandom(time(NULL));
    //port = random()%1000 + PORTRANGE_MIN;
    port=8001;
    printf("Word server listening on port %d\n", port);

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

	printf("  Server received command \"%s\" from client %s on port %d\n",
	       buf, inet_ntoa(si_client.sin_addr), ntohs(si_client.sin_port));

	if(strncmp(buf, "quit", 4) == 0)
	    quit = 1;

	if( quit == 1 )
	  sprintf(tosend, "%s", "OK");
	else if(strncmp(buf, "noun", 4) == 0)
	  {
	    choice = random()%MAX_NOUNS;
	    sprintf(tosend, "%s", nounlist[choice]);
	  }
	else if(strncmp(buf, "adj", 3) == 0)
	  {
	    choice = random()%MAX_ADJS;
	    sprintf(tosend, "%s", adjlist[choice]);
	  }
	else if(strncmp(buf, "verb", 4) == 0)
	  {
	    choice = random()%MAX_VERBS;
	    sprintf(tosend, "%s", verblist[choice]);
	  }
	else
	  {
	    choice = random()%MAX_DUMDUMS;
	    sprintf(tosend, "%s", dummylist[choice]);
	  }

	printf("    Sending back \"%s\" as a response\n", tosend);

	sendto(s, tosend, strlen(tosend), 0, client, len);
      }
    close(s);
    return 0;
  }
