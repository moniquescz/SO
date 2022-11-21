#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <time.h>

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
    
    // CREAR EL SOCKET
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
 
     portno = 9999; // DEFINIMOS QUE USE EL PUERTO 9999

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     
     // BIND
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
              error("ERROR on binding");


     // ESPERAR A QUE EL CLIENTE SE CONECTE
     printf(" > Waiting for a client...\n");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);

     // EJECUTAR EL COMANDO Y ENVIAR EL OUTPUT AL CLIENTE
     do{

        // ACEPTAR UNA NUEVA CONEXIÓN
        newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
        if (newsockfd < 0) 
            error("ERROR on accept");
        bzero(buffer,256);        
       
        // RECIBIR EL COMANDO QUE EL CLIENTE QUIERE EJECUTAR
        n = recv(newsockfd,buffer,255,0);
        if (n < 0) error("ERROR reading from socket");
        printf(" > The user wants to execute the following command: %s\n",buffer);

        // EJECUTAR EL COMANDO Y GUARDAR SU OUTPUT EN EL ARCHIVO test.txt
        if(fork() == 0){
            
            int fd = open("test.txt", O_RDONLY | O_WRONLY | O_TRUNC); // abrir el archivo para escritura

            dup2(fd, 1);   
            dup2(fd, 2);   
            close(fd);    //  hacer que stdout vaya al archivo
        
            execlp(buffer, buffer, NULL); // ejecutar el comando
            exit(0);
        }
        sleep(1);

        // MANDAR EL OUTPUT DEL COMANDO AL CLIENTE
        int fd = open("test.txt", O_RDONLY); // abrir el archivo con el output para lectura

        n = read(fd,buffer,256); 
        n = write(newsockfd,buffer,256);
        if (n < 0) error("ERROR writing to output"); //escribir el output en el socket

        close(fd);

        printf(" > Waiting for a new client...\n");

    } while (true);

    // CERRAR EL SOCKET
    printf(" > Closing socket...\n");
     close(newsockfd);
     close(sockfd);

     return 0; 
}

