# Cooperative-Scheduler

**Cooperative-Scheduler main functions**
-------------------------------------------
#### - void Dispatch() 
This function is called in the main inside while (1) and it gets handles every clock unit. \
This function dispatches task-by-task from the ready queue that are ready to execute. As the ready queue is always sorted according to the priority to run, dispatch fetch the first task in the ready queue and re-adjust the head pointer of the queue to point to the first task. \
For dispactching the ready task, the schudler uses a global pointer of typedef task and makes it point to the dispatched task to get executed.

#### - void init() 
The main purpose of this function is to initialize the *ready queue* and the *delay queue* using malloc.

#### - struct task
This struct is used to represent the *tasks* that will get scheduled using the scheduler. The struct includes:  pointer to the next task, priority of the task and integer variable, and delay associated to this task. (The delay is included in the task struct to be used in the delay queue and ReRunMe option).

#### - struct queue
This struct only contain a pointer to the head of the queue of type *task*.

#### - void QueTask(task_function * t_f, int prio)
This function is used to queue the tasks in the *ready queue* according to the task's priority. It also responsible for keeping the *ready queue* always ready. The parameters for this function are: pointer to the task to be queued in the ready queue and task priority.
