#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void DearOldDadProc(int []);
void PoorStudentProc(int[]);

int  main(int  argc, char *argv[])
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;

     ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     printf("Server has received a shared memory of four integers...\n");

     ShmPTR = (int *) shmat(ShmID, NULL, 0);
     if (*ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     printf("Server has attached the shared memory...\n");

     ShmPTR[0] = 0;
     ShmPTR[1] = 0;

     printf("Main has filled BANKACCT = %d TURN = %d in shared memory...\n", ShmPTR[0], ShmPTR[1]);

     printf("Main is about to fork child process...\n");

     pid = fork();

     if(pid < 0){
          printf("*** fork error (Main) ***\n");
          exit(1);
     }
     else if(pid == 0){
          PoorStudentProc(ShmPTR);
          exit(0);
     }
     else{
          DearOldDadProc(ShmPTR);
     }

     wait(&status);
     printf("Main has detected completion of child.\n");
     shmdt((void *) ShmPTR);
     printf("Main has detached its shared memory.\n");
     shmctl(ShmID, IPC_RMID, NULL);
     printf("Main has removed its shared memory.\n");
     printf("Main exits\n");
     exit(0);
}

void PoorStudentProc(int SharedMem[]){
     int account;
     int balance;

     srand(getpid());

     for(int i = 0 ; i < 25 ; i++){
          sleep(rand() % 6);

          account = SharedMem[0];

          while(SharedMem[1] != 1){

          };

          balance = rand() % 51;
          printf("Poor student needs %d \n", balance);

          if(balance <= account){
               account -= balance;
               printf("Poor student: withdraws $%d / Balance = $%d\n, balance, account");
          }
          else{
               printf("Poor student: Not enough cash ($d)\n", account);
          }

          SharedMem[0] = account;
          SharedMem[1] = 0;

          printf("Poor studant process started\n");
          printf("Poor student found %d %d in shared memory.\n", SharedMem[0], SharedMem[1]);
          printf("Poor student is about to exit\n");

          }
     }

     void DearOldDadProc(int SharedMem[]){
          int account;
          int balance;

          srand(getpid());

          for(int i = 0 ; i < 25 ; i++){
               sleep(rand() % 6);

               account = SharedMem[0];

               while(SharedMem[1] != 0){

               };

               if(account <= 100){
                    balance = rand() % 101;

                    if(balance % 2 == 0){
                         account += balance;
                         printf("Dear old dad: Deposits $%d / Balance = $%d \n", balance, account);
                    }
                    else{
                         printf("Dear old dad: Doesn't have any money to give\n");
                    }
                    SharedMem[0] = account;
                    SharedMem[1] = 1;
               }
               else{
                    printf("Dear old dad: Deposits $%d / Balance = $%d \n", balance, account);
               }
               printf("Dear old dad process started.\n");
               printf("Dear old dad found %d %d in shared memory.\n", SharedMem[0], SharedMem[1]);
               printf("Dear old dad is about to exit.\n");
          }
     }
