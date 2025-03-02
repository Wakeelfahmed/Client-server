#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <signal.h>
#include <mqueue.h>
#include <sys/wait.h>

#define MAX_CLIENTS 10
#define MAX_CMD_LEN 1024
#define SERVER_QUEUE_KEY 1234
#define RESPONSE_QUEUE_KEY 5678 // New queue for responses

typedef struct
{
    long msg_type;
    pid_t client_pid;
    char command[MAX_CMD_LEN];
} Message;

typedef struct
{
    pid_t pid;
    int hidden;
} Client;

Client clients[MAX_CLIENTS];
int client_count = 0;
int server_msg_queue;
int response_msg_queue; // Queue for responses
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
void register_client_shutdown(pid_t client_pid)
{
    char queue_name[64];
    mqd_t mq;
    struct mq_attr attr;

    // Construct the queue name using client's PID
    snprintf(queue_name, sizeof(queue_name), "/client_broadcast_%d", client_pid);

    // Define queue attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 256;
    attr.mq_curmsgs = 0;

    // Create the message queue
    mq = mq_open(queue_name, O_CREAT | O_WRONLY, 0644, &attr);
    if (mq == (mqd_t)-1)
    {
        perror("mq_open failed");
        return;
    }

    printf("[Child Thread * %lu]: Registered the Shutdown broadcast message queue '%s'\n", pthread_self(), queue_name);

    // Close the queue (server can reopen it when needed)
    mq_close(mq);
}
void register_client(pid_t pid)
{
    pthread_mutex_lock(&lock);
    if (client_count < MAX_CLIENTS)
    {
        clients[client_count].pid = pid;
        clients[client_count].hidden = 0;
        client_count++;
        printf("\n[Child Thread * %lu]: Registered client (PID: %d) to the client list. Total clients  ---> [%d]\n", pthread_self(), pid, client_count);
        register_client_shutdown(pid);
    }
    else
    {
        printf("[Child Thread]: Client list full. Cannot register PID: %d\n", pid);
    }
    pthread_mutex_unlock(&lock);
}

void send_response(pid_t client_pid, const char *response)
{
    Message msg;
    msg.msg_type = 1;
    msg.client_pid = client_pid;
    strncpy(msg.command, response, sizeof(msg.command) - 1);
    msg.command[sizeof(msg.command) - 1] = '\0'; // Ensure null-termination

    if (msgsnd(response_msg_queue, &msg, sizeof(Message) - sizeof(long), 0) == -1)
        perror("msgsnd response");
}
void list_clients(pid_t client_pid)
{
    pthread_mutex_lock(&lock);

    char full_list[1024] = ""; // Buffer to hold the client list
    char client_info[1024];    // Buffer for individual client information

    for (int i = 0; i < client_count; i++)
    {
        if (!clients[i].hidden)
        {
            snprintf(client_info, sizeof(client_info), "Client %d --> (PID %d)\n", i + 1, clients[i].pid);
            strcat(full_list, client_info); // Append to the full list string
        }
    }
    send_response(client_pid, full_list);
    pthread_mutex_unlock(&lock);
}
void hide_client(pid_t client_pid)
{
    pthread_mutex_lock(&lock);
    for (int i = 0; i < client_count; i++)
    {
        if (clients[i].pid == client_pid)
        {
            if (clients[i].hidden == 1)
            {
                send_response(client_pid, "You Are Already Hidden...");
                pthread_mutex_unlock(&lock);
                return;
            }
            clients[i].hidden = 1; // Mark client as hidden
            break;
        }
    }
    send_response(client_pid, "You Are Now Hidden...");
    pthread_mutex_unlock(&lock);
}
void unhide_client(pid_t client_pid)
{
    pthread_mutex_lock(&lock);
    for (int i = 0; i < client_count; i++)
    {
        if (clients[i].pid == client_pid)
        {
            if (clients[i].hidden == 0)
            {
                send_response(client_pid, "You Are Not Hidden At All...");
                pthread_mutex_unlock(&lock);
                return;
            }
            clients[i].hidden = 0; // Mark client as unhidden
            break;
        }
    }
    send_response(client_pid, "You Are Now Visible Again...");
    pthread_mutex_unlock(&lock);
}

void *handle_client(void *arg)
{
    Message msg = *(Message *)arg;
    free(arg);

    if (strcmp(msg.command, "REGISTER") == 0)
        register_client(msg.client_pid);
    else if (strcmp(msg.command, "EXIT") == 0)
    {
        pthread_mutex_lock(&lock);
        for (int i = 0; i < client_count; i++)
        {
            if (clients[i].pid == msg.client_pid)
            {
                for (int j = i; j < client_count - 1; j++)
                    clients[j] = clients[j + 1]; // Shift remaining clients down

                client_count--;
                printf("\n[Child Thread * %lu]: Cleaning up client (PID %d) resources...\n", pthread_self(), msg.client_pid);
                send_response(msg.client_pid, "Client disconnected successfully.");
                break;
            }
        }
        pthread_mutex_unlock(&lock);
    }
    else if (strcmp(msg.command, "LIST") == 0)
        list_clients(msg.client_pid);
    else if (strcmp(msg.command, "HIDE") == 0)
        hide_client(msg.client_pid);
    else if (strcmp(msg.command, "UNHIDE") == 0)
        unhide_client(msg.client_pid);
    else if (strcmp(msg.command, "exit") == 0)
        send_response(msg.client_pid, "Ignored 'exit' command as it may Exit ther Shell Session...");
    else
    {
        // Treat as a shell command
        int pipefd[2];
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            send_response(msg.client_pid, "Error creating pipe.");
            pthread_exit(NULL);
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            send_response(msg.client_pid, "Error forking process.");
        }
        else if (pid == 0)
        {
            // Child process: Redirect stdout to pipe and execute shell command
            close(pipefd[0]); // Close read end
            dup2(pipefd[1], STDOUT_FILENO);
            dup2(pipefd[1], STDERR_FILENO);
            close(pipefd[1]);

            // execvp("/bin/bash", "bash", "-c", msg.command, NULL);
            char *args[] = {"/bin/bash", "-c", msg.command, NULL};
            execvp(args[0], args);

            exit(1);
        }
        else
        {
            // Parent process: Read command output and send response
            close(pipefd[1]); // Close write end

            char buffer[55024];
            ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
            close(pipefd[0]);

            if (bytes_read > 0)
            {
                buffer[bytes_read] = '\0'; // Null-terminate output
                send_response(msg.client_pid, buffer);
            }
            else
            {
                send_response(msg.client_pid, "Command executed, but no output.");
            }

            waitpid(pid, NULL, 0); // Wait for child process
        }
    }
    pthread_exit(NULL); // Exit the child thread
}

void send_shutdown_signal(pid_t client_pid)
{
    char queue_name[64];
    mqd_t mq;

    snprintf(queue_name, sizeof(queue_name), "/client_broadcast_%d", client_pid);

    // Open the queue for writing
    mq = mq_open(queue_name, O_WRONLY);
    if (mq == (mqd_t)-1)
    {
        perror("mq_open failed");
        return;
    }

    // Send shutdown message
    const char *shutdown_msg = "SHUTDOWN";
    mq_send(mq, shutdown_msg, strlen(shutdown_msg), 0);

    mq_close(mq);
}
void shutdown_server(int signo)
{
    printf("----------------------------------------------------------------------------------------------------------\n");
    printf("[Main Thread -- %lu]: Signal 2 received...\n", pthread_self());
    printf("[Main Thread -- %lu]: Grecefully exiting...\n", pthread_self());
    printf("[Main Thread -- %lu]: Cleaning up server and client resources...\n", pthread_self());
    printf("[Main Thread -- %lu]: Broadcasting 'SHUTDOWN' message to all the clients...\n", pthread_self());
    pthread_mutex_lock(&lock);
    for (int i = 0; i < client_count; i++)
        send_shutdown_signal(clients[i].pid);

    pthread_mutex_unlock(&lock);

    msgctl(server_msg_queue, IPC_RMID, NULL);
    msgctl(response_msg_queue, IPC_RMID, NULL);
    printf("[Main Thread -- %lu]: Shutting down...\n", pthread_self());
    exit(0);
}

int main()
{
    signal(SIGINT, shutdown_server);

    printf("|################### I am the PARENT PROCESS (PID: %d) running this SERVER ##################|\n", getpid());
    printf("|---------------------------------------------------------------------------------------------|\n");

    printf("\n[Main Thread -- %lu]: I am the Server's Main Thread. My Parent Process is (PID: %d)...\n", pthread_self(), getppid());

    server_msg_queue = msgget(SERVER_QUEUE_KEY, IPC_CREAT | 0666);
    response_msg_queue = msgget(RESPONSE_QUEUE_KEY, IPC_CREAT | 0666); // Create response queue
    if (server_msg_queue == -1 || response_msg_queue == -1)
    {
        perror("msgget");
        exit(1);
    }

    printf("[Main Thread -- %lu]: Broadcast message queue & Server message queue created. Waiting for the client messages...\n", pthread_self());

    while (1)
    {
        Message msg;
        if (msgrcv(server_msg_queue, &msg, sizeof(Message) - sizeof(long), 0, 0) == -1)
        {
            perror("msgrcv");
            continue;
        }

        printf("\n[Main Thread -- %lu]: Received command '%s' from client (PID: %d). About to create a child thread.\n", pthread_self(), msg.command, msg.client_pid);

        pthread_t thread;
        Message *msg_copy = malloc(sizeof(Message));
        *msg_copy = msg;
        pthread_create(&thread, NULL, handle_client, msg_copy);

        printf("[Main Thread -- %lu]: Successfully created the child thread [%lu]\n", pthread_self(), thread);

        pthread_detach(thread);

        printf("[Main Thread -- %lu]: The child thread [ %lu ] successfully exited\n", pthread_self(), thread);
    }

    return 0;
}
