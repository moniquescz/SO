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
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage, command\n");
       exit(0);
    }

    // CREAR EL SOCKET
    portno = 9999; // especificar que vamos a usar el puerto 9999
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
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

    //CONECTAR
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting"); //connect()
    printf(" > Connected to the server\n");

    //ESCRIBIR EN EL BUFFER EL COMANDO
    strncpy(buffer,argv[2],255);

    //ENVIAR EL COMANDO AL SOCKET
    n = write(sockfd,buffer,strlen(buffer)); //send the command to the socket
    if (n < 0) 
        error("ERROR writing to socket");
    bzero(buffer,256);

    //LEER EL OUTPUT DEL COMANDO DEL SOCKET
    n = read(sockfd,buffer,255);
    if (n < 0) 
        error("ERROR reading from socket");
    printf(" > The output of the command is: \n%s",buffer);

    //CERRAR EL SOCKET    
    close(sockfd);

    return 0;
}