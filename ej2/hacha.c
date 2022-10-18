#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUF_SIZE 1024

void crear_hijos(char archivo[], char *nombre, int fd[], int bytes, int *leidos){
    //printf("Voy a crear un hijo \n");
    char buffer[bytes];

    int lee = open(archivo,O_RDONLY); //abrir el archivo
    lseek(lee,*leidos,SEEK_CUR);
    *leidos+=bytes;


    //printf("Nomobre del archivo a leer %s \n", archivo);
    if(lee == -1){
        exit(EXIT_FAILURE);
    }
    printf("Creando archivo %s ...\n", nombre);

    int bytes_read = read(lee,&buffer,bytes);
    //printf("HE LEIDO %d \n", bytes_read);
    int bytes_written = write(fd[1],&buffer,bytes_read);
    //printf("HE ESCRITO %d \n", bytes_written);

    if (fork()==0){
        //printf("Entro al if \n");
        int escribe = open(nombre,O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
        //printf("Open2 %d", escribe);
        bytes_read = read(fd[0],&buffer,bytes);
        //printf("HE LEIDO2 %d \n", bytes_read);
        bytes_written = write(escribe,&buffer,bytes_read);
        //printf("HE ESCRITO2 %d \n", bytes_written);
        close(escribe);
        kill(getpid(),SIGKILL);
    }
    
    close(lee);
    
}

void crear_nombres(int n_hijos, int hijos_creados, char *args, char *nombre){
    //printf("voy a crear un nombre \n");
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
    //printf("NOMBRE: %s \n",nombre);

}





int main(int argc, char *argv[]){
    int bytes, df, size_total, n_hijos = 0, hijos_creados = 0, bytes_leidos = 0;
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
    //printf("llego a calcular cuantos hijos \n");
    stat(argv[1], &sfile);
    size_total = sfile.st_size;
    if(size_total%bytes == 0){
        n_hijos = size_total/bytes;
    }
    else{
        n_hijos = (size_total/bytes) + 1;
    }

    //bucle para crear los hijos
    for ( int i = 0; i < n_hijos;i++){
        int fd[2];
        //printf("%d \n",i);
        
        pipe(fd); //crear el pipe que concecte el padre con ese hijo

        crear_nombres(n_hijos,hijos_creados,argv[1],nombre_archivo); //nombre_archivo = nombre del archivo creado por ese hijo
        hijos_creados++;

        crear_hijos(argv[1],nombre_archivo,fd,bytes,&bytes_leidos);


        close(fd[0]);

        sleep(3); 


    }
    printf("Fin! \n");

}
