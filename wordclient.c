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

#define MAX_BUF_LEN 512

/* Hardcode the IP address of the server (local or remote) */
/* #define SERVER_IP "127.0.0.1"   /* loopback interface */
//#define SERVER_IP "136.159.5.22"  /* rsx1.cpsc.ucalgary.ca */
#define SERVER_IP "localhost"  /* rsx1.cpsc.ucalgary.ca */

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

	/* ASk client to send file command to get file information*/
  printf("Enter FILE to know information of files in server\n");
  char message[1024];
  gets(message);

	int count;
	count = send(sock, message, sizeof(message), 0);
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

	      printf("Enter FILE name\n");
      char message[1024];
      gets(message);

      int count;
      count = send(sock, message, sizeof(message), 0);
      if (count < 0) {
        printf("Error in send()\n");
				exit(-1);
      }

  int check=0;
	int check1=0;
  	/* ASk client to send the file name*/
  while (check1==0){
      char rcv_message[1024];
      count = recv(sock, rcv_message, sizeof(rcv_message), 0);
      if (count < 0) {
        printf("Error in recv()\n");
				exit(-1);
      } else {
        printf("Server send this message: %s \n", rcv_message);
				if (strncmp(rcv_message, "DONE", 4) == 0)
					{
						check1=1;
					}
      }
  }



	/* Close the socket */
	close(sock);

    //-------------UDP ------------------------
    struct sockaddr_in si_server;
    struct sockaddr *server;
    int s, i, len = sizeof(si_server);
    char buf[MAX_BUF_LEN];
    int readBytes;
    int quit;

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
      {
	printf("Could not setup a socket!\n");
	return 1;
      }

    memset((char *) &si_server, 0, sizeof(si_server));
    si_server.sin_family = AF_INET;
    si_server.sin_port = htons(SERVER_PORT);
    server = (struct sockaddr *) &si_server;

    if (inet_pton(AF_INET, SERVER_IP, &si_server.sin_addr)==0)
      {
					printf("inet_pton() failed\n");
					return 1;
      }

    printf("This is the client side of the wordserver demo...\n");
    printf("Your choices are 'noun', 'adjective', 'verb', or 'quit'\n");

    quit = 0;
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
	buf[readBytes] = '\0'; // padding with end of string symbol
	printf("Word received from server %s on port %d is \"%s\"\n\n",
	       inet_ntoa(si_server.sin_addr), ntohs(si_server.sin_port), buf);
      }
    close(s);
    return 0;
  }
