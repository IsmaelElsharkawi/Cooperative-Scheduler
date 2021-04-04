#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

typedef void task_function(void);
typedef struct task t;
typedef struct queue q;
void QueTask(task_function * t_f, int prio);



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

void Dispatch(){
	if(ready_queue->head_of_queue==NULL) return;
	current_running_task = ready_queue->head_of_queue;/*IMPORTANT: This points to the task currently running so that it can be called from within ReRunMe */
	ready_queue->head_of_queue->f();
	ready_queue->head_of_queue = ready_queue->head_of_queue->next_task;
}



void ReRunMe(int delay){
	bool delay_flag = false;
	t * current_task;
	t * last_task;
	t * new_task;	
        new_task = (t*)malloc(sizeof(t));
        
    	new_task->f = current_running_task->f;
    	new_task->priority = current_running_task->priority;
    	new_task->delay = delay;
    	
	if(delay==0){ 
		QueTask(current_running_task->f, current_running_task->priority);
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

/*Assumption: if 2 tasks have the same priority, FCFS*/
void QueTask(task_function * t_f, /*pointer to the function*/    /*IMPORTANT: This was changed to a c string for testing purposes*/
            //char * string_new, 
             int prio /*priority of the task entered into the queue*/)
{   
    bool priority_flag = false;
    t * current_task;
    t * last_task; 
    t * new_task;
	
    new_task = (t*)malloc(sizeof(t));
    //new_task->string = string_new;
    new_task->f = t_f;
    new_task->delay = -1; //-1 is the default value of the delay, since a task in ready queue has no delay
    new_task->priority = prio; //can choose to make validations on the priority entered later  
    if(ready_queue->head_of_queue==NULL){    /*special case: the first element in the queue*/
        ready_queue->head_of_queue = new_task;
        new_task->next_task = NULL;
    }else{
        if(ready_queue->head_of_queue->priority > prio){ /*special case: the case of having a priority higher than the head of the queue*/
            new_task->next_task = ready_queue->head_of_queue;
            ready_queue->head_of_queue = new_task;
        }else{
            
            current_task = ready_queue->head_of_queue;
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




	/*This code was intended for the validation of the QueTask function*/
    /*
    bool testing_flag = false;

    t * current_p = ready_queue.head_of_queue;

    while(testing_flag == false && current_p!=NULL){
        current_p->f();
        current_p = current_p->next_task;
    }
    printf("\n");
    */
}



void Systick(){
    t * current_p;
    current_p = delay_queue->head_of_queue;

    while(current_p!=NULL){
        current_p->delay -= 1;
        if(current_p->delay<=0){
        	printf("-->%d<--\n", current_p->priority);
        	QueTask(current_p->f, current_p->priority);
        	delay_queue->head_of_queue = current_p->next_task;
        }
        current_p = current_p->next_task;
        
    }
    
    
}

bool task_1_flag=true;
bool task_2_flag=true;
void task_1(void){
    printf("task1");
    if(task_1_flag==true){
    	task_1_flag = false;
    	ReRunMe(3);
    	
    }
}

void task_2(void){
    printf("task2");
    if(task_2_flag==true){
    	task_2_flag = false;
    	ReRunMe(5);
    	
    }
}

void task_3(void){
    printf("task3");
}
void task_4(void){
    printf("task4");
}
void task_5(void){
    printf("task5");
}
int main(){

    init();
    
    QueTask(task_1, 0);
    QueTask(task_2, 0);
    QueTask(task_3, 3);
    QueTask(task_4, 4);
    QueTask(task_5, 5);

	
    for(int i =0; i<6;i++){ /*This needs to be changed into an infinite loop in order to match the project description*/
	Dispatch();
	for(int i =0; i<6;i++){
		Systick();
		//sleep(500);
	}
    }
    return 0;
}
