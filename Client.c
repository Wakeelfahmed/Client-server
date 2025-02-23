#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <signal.h>

#define MAX_CMD_LEN 100
#define SERVER_QUEUE_KEY 1234
#define RESPONSE_QUEUE_KEY 5678 // Response queue for client

typedef struct
{
    long msg_type;
    pid_t client_pid;
    char command[MAX_CMD_LEN];
} Message;

int server_msg_queue;
int response_msg_queue;
char prompt[10] = "> ";

void *listen_for_shutdown(void *arg)
{
    Message msg;
    while (1)
    {
        if (msgrcv(response_msg_queue, &msg, sizeof(Message) - sizeof(long), 0, 0) != -1)
        {
            if (strcmp(msg.command, "SHUTDOWN") == 0)
            {
                printf("[Main Thread]: Server shutdown received. Exiting...\n");
                printf("%s\n", msg.command);
                exit(0);
            }
        }
    }
    return NULL;
}

void handle_shutdown(int signo)
{
    printf("[Main Thread]: Server shutdown received. Exiting...\n");
    exit(0);
}

void send_command(char *cmd)
{
    Message msg;
    msg.msg_type = 1;
    msg.client_pid = getpid();
    strcpy(msg.command, cmd);

    if (msgsnd(server_msg_queue, &msg, sizeof(Message) - sizeof(long), 0) == -1)
    {
        perror("msgsnd");
    }
}

void receive_response()
{
    Message msg;
    if (msgrcv(response_msg_queue, &msg, sizeof(Message) - sizeof(long), 0, 0) == -1)
    {
        perror("msgrcv response");
        return;
    }
    // if(strcmp(msg.command, "SHUTDOWN") == 0)
    // {
    //     printf("[Main Thread -- %lu] Server is shutting down. Exiting...\n", pthread_self());
    //     exit(0);
    // }

    printf("[Main Thread -- %lu] Received response from server\n=====================================================================\n%s\n", pthread_self(), msg.command);
}

int main()
{
    signal(SIGINT, handle_shutdown);

    printf("|################## I am the Parent Process (PID: %d) running this Client #################|\n", getpid());
    printf("|-------------------------------------------------------------------------------------------|\n");

    server_msg_queue = msgget(SERVER_QUEUE_KEY, 0666);
    response_msg_queue = msgget(RESPONSE_QUEUE_KEY, 0666); // Open response queue
    if (server_msg_queue == -1 || response_msg_queue == -1)
    {
        perror("msgget");
        exit(1);
    }

    pthread_t shutdown_thread;
    pthread_create(&shutdown_thread, NULL, listen_for_shutdown, NULL);
    pthread_detach(shutdown_thread);

    printf("\n[Main Thread -- %lu]: I am the Client's Main Thread. My Parent Process is (PID: %d)...\n", pthread_self(), getppid());
    printf("\n[Main Thread -- %lu]: Created a Child Thread [%lu] for listening to the server's SHUTDOWN broadcast message...\n", pthread_self(), shutdown_thread);

    printf("\n[Main Thread -- %lu]: Client initialized. Enter commands (type 'EXIT' to quit)...\n", pthread_self());

    while (1)
    {
        printf("\n%s Enter Command: ", prompt);
        char command[MAX_CMD_LEN];
        fgets(command, MAX_CMD_LEN, stdin);
        command[strcspn(command, "\n")] = 0;

        if (strlen(command) == 0)
        {
            printf("Invalid input. Please enter a valid command.\n");
            continue;
        }

        if (strcmp(command, "EXIT") == 0)
        {
            send_command(command);
            printf("Exiting client...\n");
            receive_response(); // Wait and print response from server
            exit(0);
        }
        else if (strncmp(command, "CHPT ", 5) == 0)
        {
            strcpy(prompt, command + 5);
            printf("Prompt changed to '%s'\n", prompt);
        }
        else
        {
            send_command(command);
            receive_response(); // Wait and print response from server
        }
    }

    return 0;
}
