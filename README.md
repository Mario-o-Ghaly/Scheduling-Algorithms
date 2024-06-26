# Scheduling-Algorithms

This project implements and simulates various CPU scheduling algorithms in **C++**. The implemented algorithms include First-Come, First-Serve (FCFS), Shortest Remaining Time First (SRTF), Round Robin (RR), and Multi-Level Feedback Queue (MLFQ) with two levels.

## Table of Contents
- [Description](#Description)
- [Algorithms Implemented](#Algorithms-Implemented)
- [Implementation Details](#Implementation-Details)
- [How to Compile and Run](#How-to-Compile-and-Run)
- [Usage](#Usage)
- [Example Output](#Example-Output)

## Description

This project provides a simulation of different CPU scheduling algorithms to evaluate their performance based on response time, waiting time, and turnaround time. It includes a user-interactive mode where users can input process details and select the scheduling algorithm to simulate.

## Algorithms Implemented
- **First-Come, First-Serve (FCFS):** 
Processes are scheduled in the order they arrive.
Simple and straightforward, but can lead to long waiting times for processes arriving later.

- **Shortest Remaining Time First (SRTF):**
Also known as **shortest job first preemptive(SJF)**. It always selects the process with the shortest remaining time for execution.
Optimal for minimizing turnaround time but can cause high waiting times for longer processes.

- **Round Robin (RR):**
Processes are assigned CPU time in a cyclic order with a fixed time quantum.
Fair and simple, with moderate response and waiting times.

- **Multi-Level Feedback Queue (MLFQ):**
Processes can move between multiple queues with different priority levels and time quantums. Regarding this project, it is only 2 levels.
Balances between shorter response times for high-priority processes and fairness for longer processes.

## How to Compile and Run
Prerequisites to compile are having a C++ compiler (e.g., g++) and the standard C++ libraries.

Compilation:
``` sh
g++ -o scheduler main.cpp
```

Running the Simulation
``` sh
./scheduler
```
## Usage
The program offers two modes: automated testing and user interaction.

- **Automated Testing**
In the test() function, the program automatically generates processes and runs simulations for each scheduling algorithm, recording and printing the average turnaround time, response time, and waiting time. Moreover, it generates .csv files with all the recorded time analyses. This mode is not the default, so if the user wants to try it, they should go to the int main(), comment userTest() function and uncomment test() function.

- **User Interaction**
The userTest() function allows the user to select the scheduling algorithm to be simulated first. Then, the user specifies the number of processes and their arrival and burst times. This is the default mode.

## Example Output
_FCFS Simulation_
```sh
This is PID 0 at time 0
This is PID 1 at time 4
This is PID 2 at time 9

Average Turn around time = 15.6667
Average response time = 7.66667
Average waiting time = 7.66667
```

_SRTF Simulation_
```sh
This is PID: 1 at time 1 with CPUTime 2
preempted:
This is PID: 2 at time 2 with CPUTime 1
preempted:
This is PID: 1 at time 3 with CPUTime 1
This is PID: 3 at time 4 with CPUTime 3

Average Turn around time = 2.66667
Average response time = 0.333333
Average waiting time = 0.666667
```

_RR Simulation_
```sh
This is PID 0 at time 0
This is PID 1 at time 8
This is PID 2 at time 16

Average Turn around time = 17.6667
Average response time = 8.66667
Average waiting time = 9.66667
```

_MLFQ Simulation_
```sh
This is PID 0 at queue 0 at time 0
This is PID 1 at queue 0 at time 8
This is PID 2 at queue 0 at time 16
This is PID 0 at queue 1 at time 24

Average Turn around time = 19.6667
Average response time = 10.6667
Average waiting time = 10.6667
```

## Implementation Details

_Process Structure_

Each process is represented by a struct proc containing:
- PID: Process ID
- arrivalTime: Arrival time of the process
- CPUTime: remaining burst time during execution
- savedBurstTime: Original burst time for calculations after simulation
- startTime: Time at which the process starts execution
- endTime: Time at which the process finishes execution

_Scheduling Functions_
- FCFS(): Implements the First-Come, First-Serve algorithm.
- SJF(): Implements the Shortest Remaining Time First algorithm.
- RR(): Implements the Round Robin algorithm with a specified time quantum.
- MLFQ(): Implements the Multi-Level Feedback Queue algorithm with two levels and specified 2 time quantums.

_Helper Functions_
- set_procs(): Initializes processes with random arrival and burst times, given the number of processes.
- to_csv(): Exports results to a CSV file for further analysis.

_Issues_

the Shortest Remaining Time First algorithm has an issue in its implementation, that is when 2 or more processes have the same remaining execution time, the algorithm does not pick the one that arrived earlier. The choice is random, in fact.
