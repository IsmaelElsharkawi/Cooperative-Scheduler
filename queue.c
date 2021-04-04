#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

typedef void task_function(void);
typedef struct task t;
typedef struct queue q;




typedef struct task
{
    //task_function* f; /*IMPORTANT: for testing purposes, the pointer to a function was replaced by a c string*/
    char * string;
    t * next_task;  /*points to the next task in the queue*/
    int priority;   //implementations of checks for the priority
    int delay;      //only implemented in the case of the delay_queue
}t;


typedef struct queue
{
    t * head_of_queue;
}q;

q ready_queue;   /*Global definition for both queues used, there is an implicit assumption that the head_of_queue is initialized to NULL*/
q delay_queue;


void QueTask(//task_function * t_f, /*pointer to the function*/    /*IMPORTANT: This was changed to a c string for testing purposes*/
            char * string_new, 
             int prio /*priority of the task entered into the queue*/)
{   
    bool priority_flag = false;
    t * current_task;
    t * last_task;
    
    t * new_task;

    new_task = (t*)malloc(sizeof(t));
    new_task->string = string_new;
    new_task->delay = -1; //-1 is the default value of the delay, since a task in ready queue has no delay
    new_task->priority = prio; //can choose to make validations on the priority entered later  
    if(ready_queue.head_of_queue==NULL){    /*special case: the first element in the queue*/
        ready_queue.head_of_queue = new_task;
        new_task->next_task = NULL;
    }else{
        if(ready_queue.head_of_queue->priority > prio){ /*special case: the case of having a priority higher than the head of the queue*/
            new_task->next_task = ready_queue.head_of_queue;
            ready_queue.head_of_queue = new_task;
        }else{
            
            current_task = ready_queue.head_of_queue;
            while(priority_flag == false && current_task!=NULL){
                if(current_task->priority > prio){
                    priority_flag = true;
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
            if(priority_flag==false){ 
                last_task->next_task = new_task;
            }
        }
    }





    bool testing_flag = false;

    t * current_p = ready_queue.head_of_queue;

    while(testing_flag == false && current_p!=NULL){
        printf("%s --> ", current_p->string);
        current_p = current_p->next_task;
    }
    printf("\n");
}

int main(){
    
    char task_1_string[] = "string for task 1";
    char task_2_string[] = "string for task 2";
    char task_3_string[] = "string for task 3";
    char task_4_string[] = "string for task 4";
    char task_5_string[] = "string for task 5";

    QueTask(task_1_string, 0);
    QueTask(task_2_string, 7);
    QueTask(task_3_string, 0);
    QueTask(task_4_string, 2);
    QueTask(task_5_string, 3);

    bool testing_flag = false;

    t * current_p = ready_queue.head_of_queue;

    while(testing_flag == false && current_p!=NULL){
        printf("%s --> ", current_p->string);
        current_p = current_p->next_task;
    }
    return 0;
}