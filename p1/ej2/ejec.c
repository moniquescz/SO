#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>




void MuereX(int signum){
    printf("I am X (%d) and I die \n",getpid());
}
void MuereY(int signum){
    printf("I am Y (%d) and I die \n",getpid());
    
}
void MuereZ(int signum){
    printf("I am Z (%d) and I die \n",getpid());
}
void MuereB(int signum){
    printf("I am B (%d) and I die \n",getpid());
    exit(0);

}
void MuereA(int signum){
    printf("I am A (%d) and I die \n",getpid());
    exit(0);

}
void Muerepapi(int signum){
    printf("I am ejec (%d) and I die \n",getpid());
    exit(0);

}


int main(int argc, char *argv[]){
    int seconds;
    int proc, pidpapi, pidA,pidB, ppid = getpid(), estado;
    pid_t pidY,pidX;

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
        signal(SIGCHLD,Muerepapi);
        fork();
    }
    else if(ppid == getppid()){
        if(getppid() == pidpapi){
            printf("I'm the process A: my pid is %d.My father is %d \n",getpid(),getppid());
            pidA = getpid(); //MickeyHerramienta para usar más adelante
            signal(SIGCHLD,MuereA);


        }
        else{
            printf("I'm the process B: my pid is %d.My father is %d, grandfather %d \n",getpid(),getppid(),pidpapi);
            pidB = getpid();
            signal(SIGCHLD,MuereB);
            
        }
        ppid = getpid();
        if(i != 2)
            proc = fork();
        
        for(int j = 0; j < 3;j++){
            if(getpid() == pidB){
                if(j == 0)
                    pidX = fork();
                if(j == 1)
                    pidY = fork();
                if(j == 2)
                    fork();
                if(getpid() != pidB){
                    switch (j)
                    {
                    case 0:
                        printf("I'm the process X: my pid is %d.My father is %d, grandfather %d, great-grandfahter %d \n",getpid(),getppid(),pidA,pidpapi);
                        signal(SIGUSR1,MuereX);
                        pause();
                        exit(0);
                                               
                        break;
                    case 1:
                        printf("I'm the process Y: my pid is %d.My father is %d, grandfather %d, great-grandfahter %d \n",getpid(),getppid(),pidA,pidpapi);
                        signal(SIGUSR1,MuereY);
                        pause();
                        kill(pidX,SIGUSR1);
                        exit(0);
                        break;
                    case 2:
                        printf("I'm the process Z: my pid is %d.My father is %d, grandfather %d, great-grandfahter %d \n",getpid(),getppid(),pidA,pidpapi);
                        //pidZ = getpid();
                        signal(SIGALRM,MuereZ);
                        alarm(seconds);
                        pause();
                        kill(pidY,SIGUSR1);
                        exit(0);
                        break;
                
                    default:
                        break;
                    }
                }
            }
            

        }
        
    }

    
   }
sleep(seconds + 1);

}