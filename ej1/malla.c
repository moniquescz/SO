#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    pid_t malla;
    int x = 0;
    int y = 0;
    int contx = 0;

    if (argc != 3){
        printf("USO: vfork name bytes\n");
        exit(-1);
    }
    else{
        x = atoi(argv[1]);
        y = atoi(argv[2]);
    }

    int ppid = getpid();
    int newppid = getpid();

    for(int i = 0; i < y; i++){
		if (i==0){
			malla = fork();
		} else if (getpid() == ppid && malla > 0 ){
			newppid = getpid();
			fork();
		}

    }	
	for (int i=0; i<x-1; i++){
		if (getppid()==newppid){
			newppid=getpid();
			fork();
       	}
  	}
    sleep(20);
}
