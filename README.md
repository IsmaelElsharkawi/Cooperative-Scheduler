# Cooperative-Scheduler

**Cooperative-Scheduler main functions**
-------------------------------------------
#### - void Dispatch() 
This function is called in the main inside while (1) and it gets handles every clock unit. \
This function dispatches task-by-task from the ready queue that are ready to execute. As the ready queue is always sorted according to the priority to run, dispatch fetch the first task in the ready queue and re-adjust the head pointer of the queue to point to the first task. \
For dispactching the ready task, the schudler uses a global pointer of typedef task and makes it point to the dispatched task to get executed.

#### - void init() 
The main purpose of this function is to initialize the *ready queue* and the *delay queue* using malloc.

#### - void QueTask(task_function * t_f, int prio)
This function is used to queue the tasks in the *ready queue* according to the task's priority. It also responsible for keeping the *ready queue* always ready. The parameters for this function are: pointer to the task to be queued in the ready queue and task priority.


**Cooperative-Scheduler Data Structures**
-------------------------------------------
#### - struct task
This struct is used to represent the *tasks* that will get scheduled using the scheduler. The struct includes:  pointer to the next task, priority of the task and integer variable, and delay associated to this task. (The delay is included in the task struct to be used in the delay queue and ReRunMe option).

#### - struct queue
This struct only contain a pointer to the head of the queue of type *task*.


**Application 1: Temperature Monitor**
-------------------------------------------
### Application Description
The purpose of this application is to have a system that continously monitors the temperature every 30 seconds.\
The user is prompted to enter a temperature, the temperature has to have two whole numbers and two decimal places, a validation check is performed to check if the user input is in the correct format. The user can change the threshold at any point in time. If the temperature measured by the sensor is higher than the set threshold, a LED light starts blinking. 

### Validation Checks Screenshots
![image](https://user-images.githubusercontent.com/61902007/114313989-25676780-9af9-11eb-9967-2185dd6d0a61.png)

### Picture of the circuit
![WhatsApp Image 2021-04-11 at 7](https://user-images.githubusercontent.com/61902007/114316314-5056b900-9b03-11eb-990f-51b2865d2d76.jpg)


### Tasks in the Application

#### Task 1
This tasks reads the temperature, it ReRunMe every 30 seconds.
#### Task 2
This functions compares the temperature to the threshold and blinks the LED if the temperature is higher than the threshold
#### Task 3
This function reads the input from the buffer that is filled by the UART Handler. The UART Handler keeps receiving letters, when the UART Handler recieves 5 letters, meaning that the threshold was taken from the user. This functions performs rigorous validation checks to make sure that the temperature entered is acceptable.  

### Demo Video 



