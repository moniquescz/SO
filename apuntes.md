# LAB SESSION 1
## **FORK**
`int void fork()`
- a UNIX process is created by system call fork
  - process that makes the call &rarr; **parent process**
  - process created &rarr; **child process**
- the call is made by the kernel and these actions happen:
  1. allocation of a gap in the process table for the child
  2. allocation of the children PID
  3. copy of the image of the parent process
  4. allocation of the child to the status "ready to run"
  5. gives the child PID to the parent and returns a value:
      - **negative**: unsuccessful creation of child process
      - **0**: to the child process
      - **positive**: child PID to the parent process
- after the call the 2 processes have identical copies of user-level settings, only differing by the PID
- to finish the scheduler will allocate a process that can be:
  - **the parent**: return to user mode and to the point it was before the call
  - **the child**: begins to run at the same point as the parent is
  - **any other process "ready to run"** (parent and child remain "ready to run")
<br/><br/>
    ```c
    #include <unistd.h>
    #include <stdio.h>
    #include <stdlib.h>
    int main()
    {
        pid_t pid;
        pid = fork();
        switch (pid)
        {
            case –1:
                printf ("No he podido crear el proceso hijo \n");
                break;
            case 0:
                printf ("Soy el hijo, mi PID es %d y mi PPID es %d \n", getpid(), getppid());
                sleep (20);
                break;
            default:
                printf ("Soy el padre, mi PID es %d y el PID de mi hijo es %d \n", getpid(), pid);
                sleep (30);
        }
        printf ("Final de ejecución de %d \n", getpid());
        exit (0);
    }
    ```
> to run the program in background mode and check it's working with the _ps_ command, add _&_ to the program call: _creaproc &_
- two processes connected by a call fork have their own private data areas &rarr; the following program displays the even values i takes in the child process and the odd values i takes in the parent process
- **descriptor variables**: asociated to files opened when the fork call is made, they are inherited by the children
> to acces the parent PID use `getppid()` and to access the child PID use `getpid()`

## **GETPID**
`int getpid(void)`

- Returns:
  - **-1**: error
  - **current process PID**: else

## **GETPPID**
`int getppid(void)`

- Returns:
  - **-1**: error
  - **current process' parent PID**: else
<br/><br/>

## **_Process finishing_**
## **EXIT**
`void exit(int estado)`

- Ends a process with the status *status* &rarr; int that is returned to the parent process of the finished (for the parent to be able to read it we must use the call `wait()`)
- If a process ends without the call `exit()` &rarr; status undefined
- Variable *?*: status of the ending of last command
> To see the status of the ending of the last command: `echo $?`
 ## **WAIT**
 `int wait(int *status)`
 `int wait(NULL)`
 - Causes the current process to wait until one of their children dies or recieves a signal
 - Returns:
   - **-1**: error
    - **else**: PID of child who dies
  
- *status* parameter &rarr; int in which *wait()* writes info. divided in two parts:
  - **if the child ends with a call *exit()***: in the 7 least significant bits (status & 0x7F) writes 0 and the 8 most significant bits ((status >> 8) & 0xff) writes the state value
  - **if the child ends killed by a signal**: in the 7 least significant bits (status & 0x7F) it writes the signal number
  
imagen
  
```c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main ( ) {
   int estado, numero;
   switch(fork()) {
      case -1 : /* ERROR */
                perror("Error in fork");
                exit(1);
      case 0  : /* HIJO */
                numero = 13;
                printf("Soy el hijo y muero con %d...\n", numero);
                sleep(20);
                exit(numero);
      default : /* PADRE */
                wait(&estado);
valor exit
núm. Señal (0 si exit)
                 printf("Soy el padre. ");
                if ((estado & 0x7F) != 0) {
                   printf("Mi hijo ha muerto con una señal.\n");
                }
                else {
                   printf("Mi hijo ha muerto con exit(%d).\n",
                          (estado>>8) & 0xFF);
                } exit(0);
  }
}

```
