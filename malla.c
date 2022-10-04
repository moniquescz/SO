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
    printf("INserte el valor Y: ");
    scanf("%d",&y);
    /*pid_t nombres[x][y];
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            if (malla>0){
            nombres[i][j] = fork();
            sleep(10);
            }
        }
    }*/

    int ppid = getpid();
    malla = fork();

    for(int i = 0; i < y - 1;i++){

        if (getpid() == ppid && malla > 0 ){
            fork();

        }

        
    }
    for(int i = 0; i  < x - 1;i++){
        
        if(getpid() != ppid){
            fork();
        }
    }

    sleep(20);

    
    

}
