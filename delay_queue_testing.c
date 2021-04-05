#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

typedef void task_function(void);
typedef struct task t;
typedef struct queue q;




typedef struct task
{
    task_function* f; /*IMPORTANT: for testing purposes, the pointer to a function was replaced by a c string*/
    //char * string;
    t * next_task;  /*points to the next task in the queue*/
    int priority;   //implementations of checks for the priority
    int delay;      //only implemented in the case of the delay_queue
}t;


typedef struct queue
{
    t * head_of_queue;
}q;

q * ready_queue;   /*Global definition for both queues used, there is an implicit assumption that the head_of_queue is initialized to NULL*/
q * delay_queue;
t * current_running_task;

void init(){
    /*IMPORTANT: if any new data structures are created, they must be initialized here*/
    ready_queue = (q*)malloc(sizeof(q));
    delay_queue = (q*)malloc(sizeof(q));
}




void ReRunMe(task_function * t_f/*IMPORTANT: this is made for testing purposes to test the delay_queue*/,
            int prio,
             int delay){
    bool delay_flag = false;
    t * current_task;
    t * last_task;
    t * new_task;   
    new_task = (t*)malloc(sizeof(t));

    new_task->f = t_f;//current_running_task->f;
    new_task->priority = prio;//current_running_task->priority;
    new_task->delay = delay;
        
    if(delay==0){ 
        return;
    }
    
    if(delay_queue->head_of_queue==NULL){    /*special case: the first element in the queue*/
        delay_queue->head_of_queue = new_task;
        new_task->next_task = NULL;
        }else{
        if(delay_queue->head_of_queue->delay > delay){ /*special case: the case of having a priority higher than the head of the queue*/
            new_task->next_task = delay_queue->head_of_queue;
            delay_queue->head_of_queue = new_task;
        }else{
            
            current_task = delay_queue->head_of_queue;
            while(delay_flag == false && current_task!=NULL){
                if(current_task->delay > delay){
                    delay_flag = true;
                    new_task->next_task = current_task;
                    last_task->next_task = new_task;
                }else{
                    last_task = current_task;
                    current_task = current_task->next_task;
                }
        
            }
            /*special case: if the element is inserted at the end of the queue
            if the element is inserted at the end of the queue, then the priority_flag must be false
            last_task will be pointing to the last task in the queue, also new_task has next_task=NULL*/
            if(delay_flag==false){ 
                last_task->next_task = new_task;
            }
        }
        }
}


int systick= 0;


void Systick(){
    t * current_p;
    current_p = delay_queue->head_of_queue;
    printf("\nClk=%d\n", systick);
    systick+=1;
    while(current_p!=NULL){
        current_p->delay -= 1;
        
        if(current_p->delay<=0){
            printf("-->DELAY=0: THE O/P OF THE FUNCTION\n");
            //QueTask(current_p->f, current_p->priority); /*IMPORTANT: this is commented to test the delay queue*/
            current_p->f();
            delay_queue->head_of_queue = current_p->next_task;
        }else{
            printf("-->d=%d,p=%d ", current_p->delay, current_p->priority);
        }
        current_p = current_p->next_task;
        
    }
    
    
    
}

bool task_1_flag=true;
bool task_2_flag=true;
void task_1(void){
    printf("task1\n");
   
}

void task_2(void){
    printf("task2\n");
    
}

void task_3(void){
    printf("task3\n");
}
void task_4(void){
    printf("task4\n");
}
void task_5(void){
    printf("task5\n");
}
int main(){

    init();
    
   ReRunMe(task_1, 1, 10);
   ReRunMe(task_2, 2, 12);
   ReRunMe(task_3, 3, 9 );
   ReRunMe(task_4, 4, 20 );
   ReRunMe(task_5, 5, 1 );

    

    for(int i =0; i<60;i++){
        Systick();
        
    }

    return 0;
}
