#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>

#define BUF_SIZE 1024

void crear_hijos(char *nombre, int fd,int bytes){
    printf("Voy a crear un hijo \n");
    if(fork() == 0){
        printf("Entro al if \n");
        char buffer;
        int escribe;
        int bytes_read;
        escribe = open(nombre,O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
        if(escribe == -1){
            exit(EXIT_FAILURE);

        }
        do{
            printf("Entro al do \n");
            bytes_read = read(fd,&buffer,bytes);
            printf("HE LEIDO %d \n", bytes_read);
            write(escribe,&buffer,bytes_read);

        }while(bytes_read == bytes);
        printf("HE ACABAO \n");
        close(escribe);
        exit(EXIT_SUCCESS);
    }
}

void crear_nombres(int n_hijos, int hijos_creados, char *args, char *nombre){
    printf("voy a crear un nombre \n");
    char aux;
    int j;
    bool nombre_copiado = false;
    for (int i=0; i<200 && !nombre_copiado; i++){
        if (args[i]!='\0' && !nombre_copiado){
            nombre[i] = args[i];
        }else{
            nombre_copiado=true;
            j=i;
        }
    }
    nombre[j]='.';
    j++;
    nombre[j]='h';
    j++;
    if (hijos_creados<10){ //LO VA A SER
        nombre[j]='0';
        j++;
    }
    aux = hijos_creados + '0';
    nombre[j]=aux;
    j++;
    nombre[j]='\0';
    printf("NOMBRE: %s \n",nombre);
    
}





int main(int argc, char *argv[]){
    int bytes, df, size_total, n_hijos = 0, hijos_creados = 0;
    struct stat sfile; //necesario para saber el tamaño de la canción
    char nombre_archivo[200];
    int bytes_read = 0, bytes_written = 0;
    char buffer[bytes];


    if (argc != 3){
        printf("USO: vfork name bytes\n");
        exit(-1);
    }
    else{
        bytes = atoi(argv[2]); //bytes = pasado por parametro
        df = open(argv[1], O_RDONLY); //df=descriptor fichero canción
    }


    //Cálculo de cuantos hijos/archivos hay que crear
    printf("llego a calcular cuantos hijos \n");
    stat(argv[1], &sfile);
    size_total = sfile.st_size;
    if(size_total%bytes == 0){
        n_hijos = size_total/bytes;
    }
    else{
        n_hijos = (size_total/bytes) + 1;
    }

    //Crear el array de pipes
    int pipes[n_hijos*2];

    //bucle para crear los hijos
    for ( int i = 0; i < n_hijos;i++){
        printf("%d \n",i);
        
        pipe(&pipes[2*i]); //crear el pipe que concecte el padre con ese hijo
        close(pipes[2*i+1]);
        crear_nombres(n_hijos,hijos_creados,argv[1],nombre_archivo); //nombre_archivo = nombre del archivo creado por ese hijo
        hijos_creados++;

        crear_hijos(nombre_archivo,pipes[2*i],bytes);
        close(pipes[2*i]);

    }

    do{
        for(int i = 0; i < n_hijos; i++){
          bytes_written = write(pipes[(2*i) + 1],&buffer,bytes_read);
        }
        
    }while(bytes_written == bytes);







}
