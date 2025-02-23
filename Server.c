#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <signal.h>

#define MAX_CLIENTS 10
#define MAX_CMD_LEN 100
#define SERVER_QUEUE_KEY 1234
#define RESPONSE_QUEUE_KEY 5678  // New queue for responses

typedef struct {
    long msg_type;
    pid_t client_pid;
    char command[MAX_CMD_LEN];
} Message;

typedef struct {
    pid_t pid;
    int hidden;
} Client;

Client clients[MAX_CLIENTS];
int client_count = 0;
int server_msg_queue;
int response_msg_queue;  // Queue for responses
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void register_client(pid_t pid) {
    pthread_mutex_lock(&lock);
    if (client_count < MAX_CLIENTS) {
        clients[client_count].pid = pid;
        clients[client_count].hidden = 0;
        client_count++;
        printf("[Child Thread]: Registered client (PID: %d). Total clients: %d\n", pid, client_count);
    } else {
        printf("[Child Thread]: Client list full. Cannot register PID: %d\n", pid);
    }
    pthread_mutex_unlock(&lock);
}

void list_clients(pid_t client_pid) {
    pthread_mutex_lock(&lock);

    char full_list[1024] = ""; // Buffer to hold the client list
    char client_info[1024]; // Buffer for individual client information

    printf("[Child Thread]: Sending client list to PID: %d\n", client_pid);
    for (int i = 0; i < client_count; i++) {
        if (!clients[i].hidden) {
            // printf("Client %d --> (PID %d)\n", i + 1, clients[i].pid);
            snprintf(client_info, sizeof(client_info), "Client %d --> (PID %d)\n", i + 1, clients[i].pid);
            strcat(full_list, client_info); // Append to the full list string
        
        }
    }

    send_response(client_pid, full_list);

    pthread_mutex_unlock(&lock);
}

void send_response(pid_t client_pid, const char *response) {
    Message msg;
    msg.msg_type = 1;
    msg.client_pid = client_pid;
    strcpy(msg.command, response);

    if (msgsnd(response_msg_queue, &msg, sizeof(Message) - sizeof(long), 0) == -1) {
        perror("msgsnd response");
    }
}

void *handle_client(void *arg) {
    Message msg = *(Message *)arg;
    free(arg);

    if (strcmp(msg.command, "REGISTER") == 0) {
        register_client(msg.client_pid);
        send_response(msg.client_pid, "Client registered successfully.");
    } else if (strcmp(msg.command, "LIST") == 0) {
        list_clients(msg.client_pid);
        // send_response(msg.client_pid, "Client list sent.");
    } else {
        printf("[Child Thread]: Invalid command received: %s\n", msg.command);
        send_response(msg.client_pid, "Invalid command.");
    }

    pthread_exit(NULL);
}

void shutdown_server(int signo) {
    msgctl(server_msg_queue, IPC_RMID, NULL);
    msgctl(response_msg_queue, IPC_RMID, NULL);  // Remove response queue
    printf("[Main Thread]: Server shutting down...\n");
    exit(0);
}

int main() {
    signal(SIGINT, shutdown_server);

    printf("|################### I am the PARENT PROCESS (PID: %d) running this SERVER ##################|\n", getpid());
    printf("|---------------------------------------------------------------------------------------------|\n");

    server_msg_queue = msgget(SERVER_QUEUE_KEY, IPC_CREAT | 0666);
    response_msg_queue = msgget(RESPONSE_QUEUE_KEY, IPC_CREAT | 0666);  // Create response queue
    if (server_msg_queue == -1 || response_msg_queue == -1) {
        perror("msgget");
        exit(1);
    }

    while (1) {
        Message msg;
        if (msgrcv(server_msg_queue, &msg, sizeof(Message) - sizeof(long), 0, 0) == -1) {
            perror("msgrcv");
            continue;
        }

        printf("[Main Thread]: Received command '%s' from client (PID: %d).\n", msg.command, msg.client_pid);

        pthread_t thread;
        Message *msg_copy = malloc(sizeof(Message));
        *msg_copy = msg;
        pthread_create(&thread, NULL, handle_client, msg_copy);
        pthread_detach(thread);
    }

    return 0;
}
