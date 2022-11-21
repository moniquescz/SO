#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fcntl.h>
#include <sys/wait.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    int num = 0;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char signal = 'y';
    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage, command\n");
       exit(0);
    }
    portno = 9999;
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //socket()
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting"); //connect()
    printf(" > Connected to the server\n");

   
    while (signal=='y'){
        if(num != 0){
            printf("Please enter the command: ");
            bzero(buffer,256);
            fgets(buffer,255,stdin);
        }
        else{
            strncpy(buffer,argv[2],255);

        }
        n = write(sockfd,buffer,strlen(buffer)); //send the command to the socket
        if (n < 0) 
            error("ERROR writing to socket");
        bzero(buffer,256);

        n = read(sockfd,buffer,255);
        if (n < 0) 
            error("ERROR reading from socket");
        printf(" > The output of the command is: \n%s",buffer);
        
        /*
        sleep(3);

        n = read(sockfd,buffer,255);
        if (n < 0) 
            error("ERROR reading from socket");
        printf("2: %s\n",buffer);
        */

        printf(" > Do you want to execute another command ? (y/n): ");
        scanf("%c",&signal);
        num += num;
    }while(signal == 'y');
    close(sockfd);

    return 0;
}