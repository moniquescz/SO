#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    pid_t malla;
    int x = 0;
    int y = 0;
    int contx = 0;
    printf("Inserte el valor X: ");
    scanf("%d",&x);
    printf("Inserte el valor Y: ");
    scanf("%d",&y);

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
