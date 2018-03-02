/* Client to receive file by connecting to server-
 * writtent by Minh from the original file of wordclient.c
 *
 * Compile using "gcc -o wordclient wordclient.c"
 */



#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>

#define MAX_BUF_LEN 1000000


//#define SERVER_IP "136.159.16.7"  /* rsx1.cpsc.ucalgary.ca */
#define SERVER_IP "127.0.0.1"  /* loopback */

/* Edit as needed to match port of server */
#define SERVER_PORT 8001

int main(void)
  {
//while(1)
//{
    // TCP -------------------- for getting file information
    /* Address initialization */
	struct sockaddr_in server1;
	int MYPORTNUM = 8002;
	memset(&server1, 0, sizeof(server1));
	server1.sin_family = AF_INET;
	server1.sin_port = htons(MYPORTNUM);
	server1.sin_addr.s_addr = htonl(INADDR_ANY);  // For local host
	//server1.sin_addr.s_addr = inet_addr(SERVER_IP);  // For IP

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
			char savefilelist[1024];
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
					strcpy(savefilelist,rcv_message);
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
	printf("The file name is %s size is %d\n",filename, filesize);
	close(sock); // TCP


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
		char total_receive[1000000];
              char total_receive1[1000000];
              char total_receive2[1000000];
              char total_receive3[1000000];
		memset(&sub_buffer, 0, sizeof(sub_buffer));
		memset(&total_receive, 0, sizeof(total_receive));
		check=0;
		int fsize=0;
		char ack[1000];
		int ack_recv[8]={0,0,0,0,0,0,0,0};

	printf("Enter START command: ");
					scanf("%s", buf);

	if (sendto(s, buf, strlen(buf), 0, server, sizeof(si_server)) == -1)
	{
		printf("sendto failed\n");
		return 1;
	}
      int check_large=0;
      int half=0;
      int check_size=0;
      if(strcmp(filename,"256KB.txt")==0){
          check_large=4;
          half=filesize/2;
          check_size=half;
      }
      else{
          check_large=1;
          check_size=filesize;
      }
while(check_large>0){
	int remaining=0;
	int checklast=0;
	int test=0;
	memset(&ack_recv, 0, sizeof(ack_recv));// make all acknowledgement receivement to zero
    while(fsize<check_size)
      {
		  int count_ACK=0;
		  while(count_ACK<8){
			memset(&buf, 0, sizeof(buf));
			//memset(&ack_recv, 0, sizeof(ack_recv));

			if ((readBytes=recvfrom(s, buf, MAX_BUF_LEN, 0, server, &len))==-1)
						{
							printf("Read error!\n");
							quit = 1;
						}
			else{
							buf[readBytes] = '\0'; // padding with end of string symbol
					// Check if the receive is the octo-leg
							if (strlen(buf)<=10) checklast=1;
							char order[100];
							strncpy(order,buf,8);
							order[8]='\0';
							for (int i=0; i<8;i++){
								//printf("The sequence is %s and order is %s\n",sequence[i],order);
								//printf("Compare %d\ns",strcmp(sequence[i],order));


								if (strcmp(sequence[i],order)==0){
//-											printf("%d ACK=%d File size is %d\n",i,ack_recv[i],fsize);

//-											printf("Checklast %d\n",checklast);
											if(ack_recv[i]==0){ // Haven't received before
												if(checklast==1)
												{

													strncpy(sub_buffer[i],buf+8,9);
													if (fsize<filesize){
														strcat(total_receive,sub_buffer[i]);
														fsize+=1;
														ack_recv[i]=1;
														//printf("File size is %d and buf=%s\n",fsize,sub_buffer[i]);
													}
													//checklast=0;

												}
												else{

													strncpy(sub_buffer[i],buf+8,strlen(buf));
													fsize+=strlen(sub_buffer[i]);
													//remaining=filesize-fsize;
													strcat(total_receive,sub_buffer[i]);
													ack_recv[i]=1;
																									}
											}
											//printf("The buffer[%d] is %s\n",i,sub_buffer[i]);
											//if(i!=5 || test ==1){
												sprintf(ack,"ACK%d",i);
												ack[strlen(ack)]='\0';
												count_ACK+=1;
//												printf("Current count ACK %d\n",count_ACK);
//-												printf("--------------------\n");
												if (count_ACK==8) {
													if (checklast==0 && fsize!= filesize){
														count_ACK=0;
														memset(&ack_recv, 0, sizeof(ack_recv));
													}
												}
												if (sendto(s, ack, strlen(ack), 0, server, sizeof(si_server)) == -1)
												{
													printf("sendto failed\n");
													return 1;
												}
												memset(&ack, 0, sizeof(ack));
												test=0;
											//}
											//if (i==5 && test ==0) {
											//	test=1;
											//}
											break;

											memset(&sub_buffer[i], 0, sizeof(sub_buffer[i]));



								} // if of comparing the order
							}// for loop to check the sequence

							//if(fsize==filesize) break;
						//}

					} // Receiving the data from server
          printf("Current file size is %d want check %d count ACK=%d\n",fsize,check_size,count_ACK);
          if(fsize==check_size) {

            break;
          }
		  }// end of count ACK
		  printf("Current file size is %d want %d\n",fsize,filesize);
		  if(fsize==check_size) {
        fsize=0;
        break;
      }
		  memset(&sub_buffer, 0, sizeof(sub_buffer));

	  }	// End of checking file size
    if(check_large==1){
        check_large=0;
    }
    else if(check_large==4){

        strcpy(total_receive1,total_receive);
        printf("Finish first part, %d,\n%s\n",strlen(total_receive1),total_receive1);
        check_size=half-6640;
        check_large=3;
    }
    else if(check_large==3){

        strcpy(total_receive2,total_receive);
        printf("Finish second part, %d,\n%s\n",strlen(total_receive2),total_receive2);
        check_size=6640;
        check_large=2;
    }
    else if(check_large==2){
        strcpy(total_receive3,total_receive);
        printf("Finish last part, %d,\n%s\n",strlen(total_receive3),total_receive3);
        strcat(total_receive2,total_receive3);
        strcat(total_receive1,total_receive2);
        strcpy(total_receive,total_receive1);

        check_large=0;
    }
  } // End check large
						FILE *f;
						remove(filename);
						f = fopen(filename, "w");
						size_t file_size=ftell(f);
						//if (file_size < filesize) {

							fprintf(f, "%s", total_receive);
						//}
						fclose(f);

						//printf("From server: \"%s\"\n\n", total_receive);
						printf("Done for file receiving\n");


			//printf("The file list from server is %s\n",&savefilelist);
		close(s);  // UDP
		//sleep(65);
	//} main while loop



    return 0;
  }
