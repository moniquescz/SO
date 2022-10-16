#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    char buffer[10];
    char archivo[]="prueba.txt";
    int lee = open(archivo,O_RDONLY);
    printf("Abierto %d \n",lee);
    int bytes_read = read(lee,&buffer,1);
    printf("Bytes leidos %d \n",bytes_read);
}
