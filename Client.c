#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <signal.h>
#include <mqueue.h>
#include <ctype.h>

#define MAX_CMD_LEN 1024
#define SERVER_QUEUE_KEY 1234
#define RESPONSE_QUEUE_KEY 5678 // Response queue for client

typedef struct
{
    long msg_type;
    pid_t client_pid;
    char command[MAX_CMD_LEN];
} Message;

Message msg;
int server_msg_queue;
int response_msg_queue;
int shutdown_msg_queue;
char prompt[10] = "> ";

void *listen_for_shutdown(void *arg)
{
    char queue_name[64];
    mqd_t mq;
    char buffer[256];

    snprintf(queue_name, sizeof(queue_name), "/client_broadcast_%d", getpid());

    // Open the queue for reading
    mq = mq_open(queue_name, O_RDONLY);
    if (mq == (mqd_t)-1)
    {
        perror("mq_open failed");
        return NULL;
    }

    while (1)
    {
        ssize_t bytes_read = mq_receive(mq, buffer, sizeof(buffer), NULL);
        if (bytes_read > 0)
        {
            printf("\n\n------------------------------------------------------------------------------------------------\n");
            buffer[bytes_read] = '\0';

            printf("[Client Thread ** %lu]: Received broadcast message \'%s\' from Server...\n", pthread_self(), buffer);
            printf("[Client Thread ** %lu]: Gracefully exiting...\n", pthread_self());
            printf("[Client Thread ** %lu]: Resource cleanup complete...\n", pthread_self());
            printf("[Client Thread ** %lu]: Shutting down...\n", pthread_self());
            printf("------------------------------------------------------------------------------------------------\n");
            exit(0);
        }
    }

    mq_close(mq);
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
    if (msgrcv(response_msg_queue, &msg, sizeof(Message) - sizeof(long), 0, 0) == -1)
    {
        perror("msgrcv response");
        return;
    }

    printf("[Main Thread -- %lu] Received response from server\n=====================================================================\n%s\n", pthread_self(), msg.command);
}

int main()
{
    // signal(SIGINT, handle_shutdown);

    printf("|################## I am the Parent Process (PID: %d) running this Client #################|\n", getpid());
    printf("|-------------------------------------------------------------------------------------------|\n");

    server_msg_queue = msgget(SERVER_QUEUE_KEY, 0666);
    response_msg_queue = msgget(RESPONSE_QUEUE_KEY, 0666); // Open response queue
    if (server_msg_queue == -1 || response_msg_queue == -1)
    {
        perror("msgget");
        exit(1);
    }
    send_command("REGISTER");
    sleep(1); // Wait for server to register client

    printf("\n[Main Thread -- %lu]: I am the Client's Main Thread. My Parent Process is (PID: %d)...\n", pthread_self(), getppid());

    pthread_t shutdown_thread;
    pthread_create(&shutdown_thread, NULL, listen_for_shutdown, NULL);
    pthread_detach(shutdown_thread);
    printf("\n[Main Thread -- %lu]: Created a Child Thread [%lu] for listening to the server's SHUTDOWN broadcast message...\n", pthread_self(), shutdown_thread);

    printf("\n[Main Thread -- %lu]: Client initialized. Enter commands (type 'EXIT' to quit)...\n", pthread_self());

    sleep(1);
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
        else if (strcmp(command, "EXIT") == 0)
        {
            send_command(command);
            printf("Exiting client...\n");
            receive_response(); // Wait and print response from server
            exit(0);
        }
        else if (strncmp(command, "CHPT ", 5) == 0)
        {
            printf("CMD entered: \'%s\'\n", command);

            // Skip extra spaces after "CHPT "
            char *start = command + 5;
            while (*start == ' ')
                start++; // Move past all spaces

            strncpy(prompt, start, sizeof(prompt) - 1); // Copy at most size-1 characters
            prompt[sizeof(prompt) - 1] = '\0'; // Ensure null termination
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
