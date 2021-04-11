# Cooperative-Scheduler

**Cooperative-Scheduler Data Structures**
-------------------------------------------
#### struct task
This struct is used to represent the *tasks* that will get scheduled using the scheduler.\ 
The attributes are: 
-pointer to the function
-pointer to the next task
-priority of the task
-delay associated with this task. (The delay is included in the task struct to be used in the delay queue and ReRunMe option).

#### struct queue
This struct only contain a pointer to the head of the queue of type *task*. There are only 2 queues in our applications, the ready_queue and the delay_queue. 

**Cooperative-Scheduler main functions**
-------------------------------------------
#### void Dispatch() 
This function is called in the main() function inside while (1) infinite loop. \
This function dispatches task-by-task from the ready queue that are ready to execute. As the ready queue is always sorted according to the priority to run, dispatch fetches the first task in the ready queue(which has the highest priority) and re-adjusts the head pointer of the queue to point to the first task. \
For dispactching the ready task, the scheduler dispatches from the ready_queue.

#### void init() 
The main purpose of this function is to initialize the *ready queue* and the *delay queue* using malloc.

#### void QueTask(task_function * t_f, int prio)
This function is used to queue the tasks in the *ready queue* according to the task's priority. It also responsible for keeping the *ready queue* always ready. The parameters for this function are: pointer to the task to be queued in the ready queue and task priority.

#### void ReRunMe(int delay)
This function is used to enque tasks into the *delay queue* and they are sorted according to the task's delay that was passed as a parameter. In the Systick Handler, when the delay expires, the task that was inserted into the *delay queue* gets inserted into the *ready_queue* according to the priority of the task. 



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
In this demo, the temperature is measured. First, the threshold is set to 10 degree Celsius to make sure that LED blinks, after that, the threshold is 
Link to YouTube: https://drive.google.com/file/d/1yqYHBFoFnw5TClmqPVuCCKca3UBKhmye/view?usp=sharing


**Application : Parking Sensor**
-------------------------------------------

### Application Description
The purpose of this application is to generate a buzz sound that reflects how close is the car from the nearest object. \
The distance is measured every 2 ticks, and according to the measured distance the buzzer produce beeps and the duration between the beeps reflects how far is the object.

### Picture of the circuit
![parking](https://user-images.githubusercontent.com/43896028/114317878-8f3c3d00-9b0a-11eb-9c35-2791808edf8b.jpg)


### Tasks in the Application
This application consists of two main tasks: 
- #### Task 1


### Demo Video 
