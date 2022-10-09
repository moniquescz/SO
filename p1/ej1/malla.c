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
    malla = fork();
    for (int i=0; i<x-1; i++){
	 if (getppid()==newppid){
		    newppid=getpid();
	   	 fork();
		}

	}


    for(int i = 0; i < y - 1;i++){

        if (getpid() == ppid && malla > 0 ){
		newppid = getpid();
		fork();
            	for (int i=0; i<x-1; i++){
			if (getppid()==newppid){
				newppid=getpid();
				fork();
        		}
    		}
	}
    }	
    sleep(20);
}
