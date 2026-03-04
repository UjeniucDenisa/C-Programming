#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>


#define NR_THREADS 42
#define MAX_SIMULTANEOUS_THREADS 4

pthread_t threads_P6[5];

typedef struct {
    int process_nr;
    int thread_nr;
} thread_data;

 //semafoare pt sincronizarea T6.4 si T6.5
    sem_t *sem_T65_start; //semnalizeaza ca T6.5 a inceput
    sem_t *sem_T64_end; //semnaliz ca T6.4 s a terminat



   //lacat /var condit pt 2.4
    pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

    int nr_active_threads=0;//contor pt threaduri active

    sem_t *thread_limit_sem;//semafor pt a limita cate threaduri ruleaza simultan


    //semafoare pt sincronizarea intre P4 si P6( ex 2.5 )
    sem_t *sem_T44_end;//ca sa poata incepe T61
    sem_t *sem_T61_end;//ca sa poata incepe T42


    void *P4_thread(void *arg)
    {
        thread_data *data=(thread_data*)arg;
        int thread_nr=data->thread_nr;
        int process_nr=data->process_nr;

        //sincronizare
        if(thread_nr==4)
        {
            info(BEGIN,process_nr,thread_nr);
            info(END,process_nr ,thread_nr);
            sem_post(sem_T44_end);
             //semaliz ca T44 s a terminat
        }
        else if(thread_nr==2)
        {
            sem_wait(sem_T61_end);//asteapta terminarea lui T61
            info(BEGIN,process_nr,thread_nr);
            info(END,process_nr,thread_nr);

        }
        else
        {
            //thdread urile T41 si T43 ruleaza nrml
             info(BEGIN,process_nr,thread_nr);
            info(END,process_nr,thread_nr);
        }
        free(data);
        return NULL;
    }

    void *P6_thread(void *arg)
 {
    thread_data *data=(thread_data*) arg;
    int process_nr=data->process_nr;
    int thread_nr=data->thread_nr;

    // //verificare semafoare inainte de ultiliz
    // if(sem_T65_start==NULL||sem_T64_end==NULL)
    // {
    //     fprintf(stderr,"Eroare: Semafoare neinitializate in thread\n");
    //     free(data);
    //     return NULL;
    // }

    
     if (thread_nr==1) //cerinta 2.5(p4 si p6)
    {
        sem_wait(sem_T44_end); //asteapta terminarea lui T44
        info(BEGIN,process_nr,thread_nr);
        info(END,process_nr,thread_nr);
        sem_post(sem_T61_end); //semaliz terminarea lui T61
    } 
    else if(thread_nr==5)//sincr T6.4 si T6.5(cerintaa 2.3)
    {
        info(BEGIN,process_nr,thread_nr);//T65 incepe primul
        sem_post(sem_T65_start);//semnaliz ca T65 a inceput

    }
    else if(thread_nr==4)
    {
        sem_wait(sem_T65_start);//asteapta sa inceapa T65
        info(BEGIN, process_nr,thread_nr);//T64 incepe dupa T65
    }
    else
    {
        info(BEGIN,process_nr,thread_nr);//celelalte thread uri incep normal
    }

    //sincronizare pt terminarea T64 si T65
    if(thread_nr==4)
    {
        info(END,process_nr,thread_nr);//T64 se termina
        sem_post(sem_T64_end);//semnalizeaza ca T64 s a terminat
    }
    else if(thread_nr==5)
    {
        sem_wait(sem_T64_end);//T65 astepta sa se termine T64
        info(END,process_nr,thread_nr);//T65 se termina dupa T64
    }
    else
    {
        info(END,process_nr,thread_nr); //celelalte thread uri se termina normal
    }

    free(data);
    return NULL;
}


int main(){
    //initializare semafoare 
    sem_T65_start=sem_open("/sem_T65_start",O_CREAT,0644,0);
    sem_T64_end=sem_open("/sem_T64_end",O_CREAT,0644,0);
    sem_T44_end=sem_open("/sem_T44_end",O_CREAT,0644,0);
    sem_T61_end=sem_open("/sem_T61_end",O_CREAT,0644,0);

    init();

    info(BEGIN, 1, 0);
    
    //P2
    if(fork()==0)
    {
        info(BEGIN,2,0);
        //P7
        if(fork()==0)
        {
            info(BEGIN,7,0);
            info(END,7,0);
            exit(0);
        }
        //end P7
        wait(NULL);//P2 asteapta P7
        info(END,2,0);
        exit(0);
    }
    //end P2

    //P3
     if(fork()==0)
     {
        info(BEGIN,3,0);

        //P5
         if(fork()==0)
         {
            info(BEGIN,5,0);

            //P6
             if(fork()==0)
             {
                info(BEGIN,6,0);

                //creeare thread-uri P6
                for(int i=1;i<=5;i++)
                {
                    thread_data *data=malloc(sizeof(thread_data));
                    data->thread_nr=i;
                    data->process_nr=6;
                    pthread_create(&threads_P6[i-1],NULL,P6_thread,data);

                }

                //asteptam threadurile dim P6
                for(int i=0;i<5;i++)
                {
                    pthread_join(threads_P6[i],NULL);
                }


                info(END,6,0);

                //curatare resurse
                sem_close(sem_T65_start);
                sem_close(sem_T64_end);
                sem_unlink("/sem_T65_start");
                sem_unlink("/sem_T64_end");

                exit(0);
             }
             //end P6
             wait(NULL);//P5 asteapta P6
            info(END,5,0);
            exit(0);
         }
         //end P5

         //P8
             if(fork()==0)
             {
                info(BEGIN,8,0);
                info(END,8,0);
                exit(0);
             }
             //end P8

        wait(NULL);//P3 asteapta P5
        wait(NULL);//P3 asteapta P8
        info(END,3,0);
        exit(0);
     }
     //end P3

     //P4
     if(fork()==0)
     {
      info(BEGIN,4,0);
      //init semafoarele 
      // sem_T44_end=sem_open("/sem_T44_end",O_CREAT,0644,0);
      // sem_T61_end=sem_open("/sem_T61_end",O_CREAT,0644,0);

      pthread_t threads_P4[4];
      thread_data *data_P4[4];

      //creem cele 4 threaduri in P4
      for(int i=1;i<=4;i++)
      {
        data_P4[i-1]=malloc(sizeof(thread_data));
        data_P4[i-1]->process_nr=4;
        data_P4[i-1]->thread_nr=i;
        pthread_create(&threads_P4[i-1],NULL,P4_thread,data_P4[i-1]);
      }

      //asteptam toate threadurile sa se termine
      for(int i=0;i<4;i++)
      {
        pthread_join(threads_P4[i],NULL);
      }

      //curatam
      sem_close(sem_T44_end);
      sem_close(sem_T61_end);
      sem_unlink("/sem_T44_end");
      sem_unlink("/sem_T61_end");


      info(END,4,0);
      exit(0);
     }
     //end P4

    wait(NULL);//P1 asteapta P2
    wait(NULL);//P1 asteapta P3
    wait(NULL);//P1 asteapta P4
    info(END, 1, 0);
    return 0;
}