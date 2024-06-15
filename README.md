# Scheduling-Algorithms
This project implements and simulates various CPU scheduling algorithms in **C++**. The implemented algorithms include First-Come, First-Serve (FCFS), Shortest Remaining Time First (SRTF), Round Robin (RR) with a quantum of 8, and Multi-Level Feedback Queue (MLFQ) with two levels (quantums of 8 and 16).

Table of Contents
Description
Algorithms Implemented
How to Compile and Run
Usage
Implementation Details
Example Output
Description
This project provides a simulation of different CPU scheduling algorithms to evaluate their performance based on response time, waiting time, and turnaround time. It includes a user-interactive mode where users can input process details and select the scheduling algorithm to simulate.

Algorithms Implemented
First-Come, First-Serve (FCFS):

Processes are scheduled in the order they arrive.
Simple and straightforward, but can lead to long waiting times for processes arriving later.
Shortest Remaining Time First (SRTF):

Always selects the process with the shortest remaining time for execution.
Optimal for minimizing turnaround time but can cause high waiting times for longer processes.
Round Robin (RR):

Processes are assigned CPU time in a cyclic order with a fixed time quantum (8).
Fair and simple, with moderate response and waiting times.
Multi-Level Feedback Queue (MLFQ):

Processes can move between multiple queues with different priority levels and time quantums (8 and 16).
Balances between shorter response times for high-priority processes and fairness for longer processes.
How to Compile and Run
Prerequisites
A C++ compiler (e.g., g++)
Standard C++ libraries
Compilation
sh
Copy code
g++ -o scheduler main.cpp
Running the Simulation
sh
Copy code
./scheduler
Usage
The program offers two modes: automated testing and user interaction.

Automated Testing
In the test() function, the program automatically generates processes and runs simulations for each scheduling algorithm, recording and printing the average turnaround time, response time, and waiting time.

User Interaction
The userTest() function allows users to input the number of processes and their arrival and burst times. The user can then select the scheduling algorithm to simulate.

To switch to user interaction mode, uncomment the userTest() call in the main() function and comment out the test() call.

cpp
Copy code
int main() {
    srand(time(NULL));
    // userTest();
    test();
    return 0;
}
Implementation Details
Process Structure
Each process is represented by a struct proc containing:

PID: Process ID
arrivalTime: Arrival time of the process
CPUTime: Burst time of the process
savedBurstTime: Original burst time for reference
startTime: Time at which the process starts execution
endTime: Time at which the process finishes execution
Scheduling Functions
FCFS(): Implements the First-Come, First-Serve algorithm.
SJF(): Implements the Shortest Remaining Time First algorithm.
RR(): Implements the Round Robin algorithm with a specified time quantum.
MLFQ(): Implements the Multi-Level Feedback Queue algorithm with two levels and specified time quantums.
Helper Functions
set_procs(): Initializes processes with random arrival and burst times.
to_csv(): Exports results to a CSV file for further analysis.
Example Output
FCFS Simulation
css
Copy code
This is PID 0 at time 0
This is PID 1 at time 4
This is PID 2 at time 9

Average Turn around time = 15.6667
Average response time = 7.66667
Average waiting time = 7.66667
RR Simulation
css
Copy code
This is PID 0 at time 0
This is PID 1 at time 8
This is PID 2 at time 16

Average Turn around time = 17.6667
Average response time = 8.66667
Average waiting time = 9.66667
MLFQ Simulation
css
Copy code
This is PID 0 at queue 0 at time 0
This is PID 1 at queue 0 at time 8
This is PID 2 at queue 0 at time 16
This is PID 0 at queue 1 at time 24

Average Turn around time = 19.6667
Average response time = 10.6667
Average waiting time = 10.6667
Conclusion
This project provides a comprehensive simulation of various CPU scheduling algorithms, allowing for performance comparison based on different metrics. It highlights the strengths and weaknesses of each algorithm and provides a hands-on approach to understanding CPU scheduling in operating systems.
