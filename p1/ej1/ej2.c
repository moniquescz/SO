#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>




void MuereX(int signum){
    printf("Me suicido soy X");


}
void MuereY(int signum){
    
}
void MuereZ(int signum){
    
}







int main(int argc, char *argv[]){
    int seconds;
    int proc, pidpapi, pidA,pidB,pidZ, ppid = getpid();

    if (argc != 2){
        printf("USO: vfork seconds\n");
        exit(-1);
    }
    else{
        seconds = atoi(argv[1]);
    }

    /*for(int i = 0; i < 2; i++){
        pid = fork();
        if(pid == 0 ){
            printf("I'm the process A: my pid is %d.My father is %d \n",getpid(),getppid());
        }
        else{
             printf("I'm the process ejec: my pid is %d \n",getpid());
             exit(0);


        }
    }
    */

   for(int i = 0; i< 3; i++){
    if(i == 0){
        pidpapi = getpid();
        printf("I'm the process ejec: my pid is %d \n",getpid());
        fork();
    }
    else if(ppid == getppid()){
        if(getppid() == pidpapi){
            printf("I'm the process A: my pid is %d.My father is %d \n",getpid(),getppid());
            pidA = getpid(); //MickeyHerramienta para usar más adelante

        }
        else{
            printf("I'm the process B: my pid is %d.My father is %d, grandfather %d \n",getpid(),getppid(),pidpapi);
            pidB = getpid();
            
        }
        ppid = getpid();
        if(i != 2)
            proc = fork();
        
        for(int j = 0; j < 3;j++){
            if(getpid() == pidB){
                fork();
                if(getpid() != pidB){
                    switch (j)
                    {
                    case 0:
                        printf("I'm the process X: my pid is %d.My father is %d, grandfather %d, great-grandfahter %d \n",getpid(),getppid(),pidA,pidpapi);
                        signal(SIGALRM,MuereX);
                        alarm(seconds);
                        break;
                    case 1:
                        printf("I'm the process Y: my pid is %d.My father is %d, grandfather %d, great-grandfahter %d \n",getpid(),getppid(),pidA,pidpapi);
                        break;
                    case 2:
                        printf("I'm the process Z: my pid is %d.My father is %d, grandfather %d, great-grandfahter %d \n",getpid(),getppid(),pidA,pidpapi);
                        pidZ = getpid();
                        break;
                
                    default:
                        break;
                    }
                }
            }
            

        }
        
    }

    
   }








    sleep(15);


}