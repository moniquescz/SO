#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>


int main(int argc, char *argv[]){
    int x,y,proc,ppid,posi = 0, posj = 0,ultimo;
        
    if (argc != 3){
        printf("USO: vfork name bytes\n");
        exit(-1);
    }
    else{
        x = atoi(argv[1]);
        y = atoi(argv[2]);
    }
    int nx = x + 1;
    int ny = y;
    int *pidx; //Lista con todos los pids de x, se compartirá por memoria para poder enviar los mensajes
    int *pidy; //Lo mismo 
    int shmidx,shmidy;

    //Compartimos las variables pidx y pidy
    if((shmidx=shmget(IPC_PRIVATE,sizeof(int)*nx,IPC_CREAT|0777))==-1){
        perror("Error al crear la memoria compartida: \n");
        return 1;
    }
    pidx = (int *) shmat (shmidx,0,0);
    if((shmidy=shmget(IPC_PRIVATE,sizeof(int)*ny,IPC_CREAT|0777))==-1){
        perror("Error al crear la memoria compartida: \n");
        return 1;
    }
    pidy = (int *) shmat (shmidy,0,0);

    pidx[0] = getpid(); //Pid Padre guardado en 0

    
    for(int i = 0; i < x; i++){
        if (i==0){
            ppid = fork();
            if(pidx[0] != getpid())
                pidx[i+1] = getpid(); //Primer hijo guardado en [1]
        }else if (ppid == 0){
            if(getpid() != pidx[0])
                pidx[i] = getpid(); //Guarda hijo que entra hasta posición x - 1
			ppid=fork();
            if(getpid() != pidx[x-1] && i == x-1)
                pidx[x] = getpid();
            if (i==x-1){
                ultimo = getpid();
            }
		}
    }	
    //Creación y
    sleep(1);
    if(getpid() == pidx[x]){
        printf("Un momento, estoy creando los hijos... \n");
        for (int i=0;i<y;i++){
            if (getppid()!=pidx[x]){
                int aux = fork();
                if(aux == 0)
                    pidy[i] = getpid();
            }
        }
    }
    //Segundos para comprobar el árbol
    sleep(5);
    


    //printar mensajes
    if(getpid() == pidx[0]){
        printf("I am the superfather(%d):my final children are:",getpid());
        for(int i = 0; i < ny;i++){
            if(i != ny - 1){
                printf("%d,",pidy[i]);
            }
            if(i == ny - 1){
                printf("%d \n",pidy[i]);
            }
        }

    }

    for(int i = 0; i < ny;i++){
        if(getpid() == pidy[i]){
            printf("I am the subchild %d, my parents are:",getpid());
            for(int i = 0; i < nx; i++){
                if(i != 0 && i != nx - 1){
                    printf("%d,",pidx[i]);
                }
                if(i == nx-1){
                    printf("%d \n",pidx[i]);
                }
            }
        }
    }
    
   //Segundos para poder comprobar que los mensajes y los pid coincides con los del árbol
   if(getpid() == pidy[y-1])
        printf("Finalizando programa... \n");
    sleep(5);
    exit(0);

}