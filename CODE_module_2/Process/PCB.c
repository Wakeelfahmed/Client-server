/******************************************************************************
 * File:PCB.c
 * 
 * Purpose:
 * This program simulates the execution of multiple processes using a 
 * Process Control Block (PCB) structure, mimicking the behavior of processes 
 * in a multitasking operating system. Each process alternates between CPU bursts 
 * and I/O bursts for a random number of cycles, demonstrating process execution 
 * and waiting times.
 * 
 * Key Concepts Demonstrated:
 * 1. Representation of processes using a Process Control Block (PCB).
 * 2. Alternating CPU bursts and I/O bursts for processes.
 * 3. Randomized execution times for bursts to simulate real-world scenarios.
 * 4. Sequential simulation of multiple processes.
 * 
 * Program Execution:
 * - A fixed number of processes are represented using the PCB structure.
 * - Each process alternates between CPU bursts (using the CPU) and I/O bursts 
 *   (waiting for I/O operations) for a random number of bursts.
 * - The duration of each CPU and I/O burst is randomly determined within 
 *   a specified range.
 * - The program outputs messages indicating the start and end of each burst 
 *   for every process.
 * 
 * Output:
 * - Start and completion messages for each CPU and I/O burst for every process.
 * - Completion messages for each process after all bursts are executed.
 * 
 * Usage:
 * - Compile: gcc PCB.c -o process
 * - Execute: ./process
 * 
 * Experimentation Instructions:
 * 1. Modify the `num_processes` variable to simulate a different number of processes.
 * 2. Adjust the random range of CPU and I/O bursts to observe different behaviors.
 * 
 * 
 *  
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Define the Process Control Block (PCB) structure
typedef struct PCB {
    int process_id;       // Unique identifier for the process
    int cpu_burst_time;   // Duration of each CPU burst
    int io_burst_time;    // Duration of each I/O burst
    int num_bursts;       // Total number of bursts (CPU + I/O cycles)
} PCB;

/**
 * Function to simulate a CPU burst.
 * @param pcb: Pointer to the PCB of the process.
 * Simulates a process utilizing the CPU for the duration specified in its PCB.
 */
void cpu_burst(PCB *pcb) {
    printf("Process %d is executing CPU burst for %d seconds...\n", 
           pcb->process_id, pcb->cpu_burst_time);
    sleep(pcb->cpu_burst_time); // Simulate the time spent on the CPU
    printf("Process %d finished CPU burst.\n", pcb->process_id);
}

/**
 * Function to simulate an I/O burst.
 * @param pcb: Pointer to the PCB of the process.
 * Simulates a process waiting for an I/O operation for the duration specified in its PCB.
 */
void io_burst(PCB *pcb) {
    printf("Process %d is executing I/O burst for %d seconds...\n", 
           pcb->process_id, pcb->io_burst_time);
    sleep(pcb->io_burst_time); // Simulate the time spent waiting for I/O
    printf("Process %d finished I/O burst.\n", pcb->process_id);
}

/**
 * Function to simulate the execution of a process.
 * @param pcb: Pointer to the PCB of the process.
 * Alternates between CPU bursts and I/O bursts for the specified number of bursts.
 */
void simulate_process(PCB *pcb) {
    for (int i = 0; i < pcb->num_bursts; i++) {
        cpu_burst(pcb); // Simulate a CPU burst
        io_burst(pcb);  // Simulate an I/O burst
    }
    printf("Process %d completed all bursts.\n", pcb->process_id);
}

/**
 * Main function to simulate multiple processes.
 * Initializes PCBs for a fixed number of processes and simulates their execution.
 */
int main() {
    srand(time(NULL)); // Initialize random seed for burst times and number of bursts

    int num_processes = 3; // Number of processes to simulate
    PCB pcbs[num_processes]; // Array to hold the PCBs of the processes

    // Initialize the PCBs with random values
    for (int i = 0; i < num_processes; i++) {
        pcbs[i].process_id = i + 1;                      // Assign a unique process ID
        pcbs[i].cpu_burst_time = rand() % 3 + 1;         // Random CPU burst: 1-3 seconds
        pcbs[i].io_burst_time = rand() % 3 + 1;          // Random I/O burst: 1-3 seconds
        pcbs[i].num_bursts = rand() % 4 + 1;             // Random number of bursts: 1-4
    }

    printf("CPU Burst and I/O Burst Simulation with PCBs:\n");

    // Simulate each process sequentially
    for (int i = 0; i < num_processes; i++) {
        printf("\nStarting simulation for Process %d...\n", pcbs[i].process_id);
        simulate_process(&pcbs[i]); // Simulate the process using its PCB
        printf("Process %d simulation complete.\n", pcbs[i].process_id);
    }

    printf("\nAll processes have completed their simulation.\n");
    return 0;
}
