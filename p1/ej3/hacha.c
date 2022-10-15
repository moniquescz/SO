#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>

#define BUF_SIZE 1024

void crear_hijos(char *nombre, int fd,int bytes){
    printf("Voy a crear un hijo \n");
    if(fork() == 0){
        printf("Entro al if \n");
        char buffer[bytes];
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
    return;
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
    int bytes,df, size_total, n_hijos = 0,hijos_creados = 0;
    struct stat sfile;
    char nombre_archivo[200];
    int *pipes;
    int bytes_read = 0;
    char buffer[bytes];


    if (argc != 3){
        printf("USO: vfork name bytes\n");
        exit(-1);
    }
    else{
        bytes = atoi(argv[2]);
        df = open(argv[1], O_RDONLY);

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
    pipes = malloc(sizeof(int) * n_hijos * 2);
    for ( int i = 0; i < n_hijos;i++){
        printf("%d \n",i);
        pipe(&pipes[2*i]);
        crear_nombres(n_hijos,hijos_creados,argv[1],nombre_archivo);
        hijos_creados++;
        crear_hijos(nombre_archivo,pipes[2*i],bytes);
        close(pipes[2*i]);

    }

    do{
        bytes_read = read(df,&buffer,bytes);
        for(int i = 0; i < n_hijos; i++){
            write(pipes[(2*i) + 1],&buffer,bytes_read);
        }

    }while(bytes_read == bytes);

    while(n_hijos){
        wait(NULL);
        n_hijos--;
    }


    






}
