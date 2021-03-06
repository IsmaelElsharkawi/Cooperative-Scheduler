# Cooperative-Scheduler

**Description**
-------------------------------------------
This scheduler runs tasks according to their priorities. The priorities are between 0 and 7. The highest priority task runs first. There is an option for delaying a task from running for a specified number of ticks. This scheduler does not support a task pre-empting another task. 

1-  **Cooperative-Scheduler Data Structures**
-------------------------------------------
#### struct task
This struct is used to represent the *tasks* that will get scheduled using the scheduler.\ 
The attributes are:
- pointer to the function
- pointer to the next task
- priority of the task
- delay associated with this task. (The delay is included in the task struct to be used in the delay queue and ReRunMe option).

####  struct queue
This struct only contain a pointer to the head of the queue of type *task*. There are only 2 queues in our applications, the ready_queue and the delay_queue. Tasks are created dynamically, i.e. there is no preset value for the number of tasks. If the head_of_queue is NULL, then there are no tasks to run. This is an example for how the *read_queue* is laid out in memory. 

![image](https://user-images.githubusercontent.com/61902007/114317881-98c5a500-9b0a-11eb-9e86-e75be2613e6a.png)

<br />
<br />


2- **Cooperative-Scheduler main functions**
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


<br />
<br />

3- **Application (1): Temperature Monitor**
-------------------------------------------
### Application Description
The purpose of this application is to have a system that continously monitors the temperature every 30 seconds.\
The user is prompted to enter a temperature, the temperature has to have two whole numbers and two decimal places, a validation check is performed to check if the user input is in the correct format. The user can change the threshold at any point in time. If the temperature measured by the sensor is higher than the set threshold, a LED light starts blinking. 

### Validation Checks Screenshots
![image](https://user-images.githubusercontent.com/61902007/114313989-25676780-9af9-11eb-9967-2185dd6d0a61.png)

### Picture of the circuit
![WhatsApp Image 2021-04-11 at 7](https://user-images.githubusercontent.com/61902007/114316314-5056b900-9b03-11eb-990f-51b2865d2d76.jpg)


### Tasks in the Application

- #### Task 1
This tasks reads the temperature, it ReRunMe every 30 seconds.
- #### Task 2
This functions compares the temperature to the threshold and blinks the LED if the temperature is higher than the threshold
- #### Task 3
This function reads the input from the buffer that is filled by the UART Handler. The UART Handler keeps receiving letters, when the UART Handler recieves 5 letters, meaning that the threshold was taken from the user. This functions performs rigorous validation checks to make sure that the temperature entered is acceptable.  

### Demo Video 
In this demo, the temperature is measured. First, the threshold is set to 10 degree Celsius to make sure that LED blinks, after that, the threshold is 
Link to YouTube: https://drive.google.com/file/d/1yqYHBFoFnw5TClmqPVuCCKca3UBKhmye/view?usp=sharing


<br />
<br />

4- **Application (2) : Parking Sensor**
-------------------------------------------

### Application Description
The purpose of this application is to generate a buzz sound that reflects how close is the car from the nearest object. \
The distance is measured every 2 ticks, and according to the measured distance the buzzer produce beeps and the duration between the beeps reflects how far is the object.

### Picture of the circuit
![parking](https://user-images.githubusercontent.com/43896028/114317878-8f3c3d00-9b0a-11eb-9c35-2791808edf8b.jpg)


### Tasks in the Application
This application consists of two main tasks: 
- #### Task 1
This measures the distance using the ultrasonic sensor by calculating the time between the trig signal and the echo signal and then divide the time by 2 and multiply by the velocity to get the distance between the detected object and the sensor. \
The task then call ReRunMe function with delay 2 ticks to measure the distance periodically every 2 ticks.

- #### Task 2
This task generate the buzz sound by setting an output GPIO pin to high then drop it back to low. This buzz sound is repeated using the *ReRunMe* of this task. *ReRunMe* takes delay as a parameter relative to how far is the object from the Ultrasonic sensor. 

### Demo Video 
In this demo the wall represents the object and the Ultrasonic sensor is used to measure the distance between the wall & the sensor. \
When the distance between the wall and the sensor decreases, the buzzer beeps more frequenlty. When the sensor is far from the wall, the time between the beeps decreases according to the distance between the object and the sensor. 

Link to video: https://drive.google.com/file/d/1ghofU_KUOK_UoHgdAoAltKYysoxb0ShJ/view?usp=sharing

<br />
<br />

5- **Testing**
-------------------------------------------
There were 4 stages for testing:
- Unit testing for the Queue.c file which has the logic for the scheduler
- Sensor testing
- Integration testing
- Application testing

#### 5.1 **Unit test for the Queue.c**
This is the testing for the 4 functions(mentioned above) and the 2 data structures used. For testing the init() and the Dispatch(), no sperate tests for these would be needed since these would be implicitly tested in the testing of init() and Dispatch(). Dispatch cannot be tested unless there is already something in the ready_queue, and thus it is necessary to have the anything to Dispatch without having any tasks queued using QueTask. Also, the init() function can't be verified if it is working correclty unless tasks are executed correctly.
##### 5.1.1 Testing QueTask()
The first test was testing the init() QueTask() and Dispatching these tasks. In order to make it easy, for testing purposes, the task had a new string attribute and the Dispatch only printed this string attribute instead of executing the function. There is a file for this in the testing directory, there is a screenshot for the output of the program. 

##### 5.1.2 Testing ReRunME()
The second test was testing the ReRunMe using delay_queue. The systick was simulated and the queued tasks each tick were printed and when it was the turn for the execution of a task, it was executed. There is a file for this in the testing directory, there is a screenshot for the output of the program. 

#### 5.2 **Unit test for the sensors**

##### 5.2.1 Testing the Temperature Sensor
In this part we started by connecting the DS3231 to measure the temperature. We used the I2C synchronous serial commuincation to commuincate between the sensor and the microcontroller. The I2C master is used to transmit to the register in the sensor responsible for detecting the temperature. There are two registers, one for getting the most significant bit of the temp reading and the other for getting the other for the least significant bit.

##### 5.2.2 Testing the Ultrasonic Sensor
In this part we tested the ultrasonic sensor using the debug mode in Keil to get the distance.

#### 5.3 **Integration Testing**
The integration testing was integrating our .c files into the CUBEMX environment and running the code within microKeil. The first thing we tried was having simple tasks, where each tasks blinks the LED for a pre-determined number of times. For example, task 1 blinks 1 time, task 2 blinks 2 times, etc. We tried having ReRunMe instances withing tasks. The folder for testing this integration is named Testing_LEDs under the integration testing directory.

#### 5.4 **Application Testing**
This is dicussed extensively in earlier sections and supported with videos for the implementations. 
