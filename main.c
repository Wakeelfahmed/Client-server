#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
// Function executed by the child thread
void *child_thread_function(void *arg)
{
    int limit = *(int *)arg; // Retrieve the argument (cast void* to int*)
    printf("child thread: started counting from 1 to %d.\n", limit);
    // Loop to print numbers from 1 to the given limit for

    for (int i = 1; i <= limit; i++)
    {
        printf("child thread: %d\n", i);
        sleep(1); // simulate some delay
    }
    printf("child thread: Finished counting.\n");
    pthread_exit(NULL); // Exit the thread
}
int main()
{
    pthread_t thread_id;  // Thread identifier
    int count_limit = 10; // Argument to pass to the child thread
    // Create a child thread
    if (pthread_create(&thread_id, NULL, child_thread_function, &count_limit) != 0)
    {
        perror("Failed to create thread");
        exit(EXIT_FAILURE);
    }
    // Detach the thread so it can run independently
    // Explore the differnce between pthread_detach() and pthread_join() if
    if (pthread_detach(thread_id) != 0)
    {
        perror("Failed to detach thread");
        exit(EXIT_FAILURE);
    }
    // Main thread continues working independently
    printf("Main thread: child thread is detached and running in background.\n");
    // simulate main thread work
    for (int i = 1; i <= 5; i++)
    {
        printf("Main thread: Doing work %d...\n", i);
        sleep(1);
    }
    printf("Main thread: Finished. Exiting without waiting for the child thread");
    return 0;
}