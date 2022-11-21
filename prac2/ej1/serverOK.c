#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/wait.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     int pid;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = 9999;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     //Esperando a cliente
     printf(" > Waiting for a client...\n");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     //recibiendo mensaje creo
     bzero(buffer,256);
     
     //Después de cliente

        n = read(newsockfd,buffer,255);
        if (n < 0) error("ERROR reading from socket");
        printf(" > The user wants to execute the following command: %s\n",buffer);

        //EJECUTAR EL COMANDO

        if(fork() == 0){
            
            int fd = open("test.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

            dup2(fd, 1);   // make stdout go to file
            dup2(fd, 2);   // make stderr go to file - you may choose to not do this
                        // or perhaps send stderr to another file

            close(fd);     // fd no longer needed - the dup'ed handles are sufficient
        
            execlp(buffer, buffer, NULL); 
            exit(0);
        }
        sleep(1);

        int fd = open("test.txt", O_RDONLY);
        n = read(fd,buffer,256);
        /*
        n = write(newsockfd,"The output of the command is: \n",sizeof("The output of the command is: \n"));
        if (n < 0) error("ERROR writing to socket");
        */
        n = write(newsockfd,buffer,256);
        if (n < 0) error("ERROR writing to output");
        
        close(fd);


    printf(" > Closing socket...\n");
     close(newsockfd);
     close(sockfd);

     return 0; 
}

