#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_shared {
    pthread_mutex_t print_lock;
    long start_time;
    int running;
} t_shared;

typedef struct s_data {
    t_shared shared;
} t_data;

// Function to get the current time in milliseconds
long get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

// Function to print messages safely
void print_message(t_data *data, int philo_id, char *message)
{
    long timestamp;
    int running;

    pthread_mutex_lock(&data->shared.print_lock);
    running = data->shared.running;
    if (running)
    {
        timestamp = get_time() - data->shared.start_time;
        printf("%ld %d %s\n", timestamp, philo_id, message);
    }
    pthread_mutex_unlock(&data->shared.print_lock);
}

// Example thread function for a philosopher
void *philosopher(void *arg)
{
    t_data *data = (t_data *)arg;
    int philo_id = 1; // Example philosopher ID

    print_message(data, philo_id, "is thinking");
    usleep(500000); // Simulating thinking time
    print_message(data, philo_id, "is eating");
    usleep(500000); // Simulating eating time
    print_message(data, philo_id, "is sleeping");
    usleep(500000);
    print_message(data, philo_id, "is die");

    
    return NULL;
}

int main(void)
{
    t_data data;
    pthread_t thread;

    // Initialize shared data
    pthread_mutex_init(&data.shared.print_lock, NULL);
    data.shared.start_time = get_time();
    data.shared.running = 1; // Set to 1 to allow messages

    // Create philosopher thread
    pthread_create(&thread, NULL, philosopher, (void *)&data);

    // Wait for the philosopher to finish
    pthread_join(thread, NULL);

    // Destroy mutex
    pthread_mutex_destroy(&data.shared.print_lock);

    return 0;
}
