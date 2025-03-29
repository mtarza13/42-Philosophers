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

// Get current time in milliseconds
long get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

// Print message with timestamp (thread-safe)
void printf_messege(t_data *data, int philo_id, char *messege)
{
    long timestamp;

    pthread_mutex_lock(&data->shared.print_lock);
    if (data->shared.running)
    {
        timestamp = get_time() - data->shared.start_time;
        printf("%ld | Philosopher %d | %s\n", timestamp, philo_id, messege);
    }
    pthread_mutex_unlock(&data->shared.print_lock);
}

// Check if simulation is running (thread-safe)
int simulation_running(t_data *data)
{
    int status;
    pthread_mutex_lock(&data->shared.print_lock);
    status = data->shared.running;
    pthread_mutex_unlock(&data->shared.print_lock);
    return status;
}

// Simulate philosopher actions
void *philosopher(void *arg)
{
    t_data *data = (t_data *)arg;
    int id = pthread_self() % 2 + 1; 

    while (simulation_running(data))  // Check if simulation is active
    {
        printf_messege(data, id, "is thinking");
        usleep(500 * 1000); // 500ms

        printf_messege(data, id, "is eating");
        usleep(1000 * 1000); // 1s

        // Simulate philosopher dying after 2s
        if (get_time() - data->shared.start_time > 2000)
        {
            pthread_mutex_lock(&data->shared.print_lock);
            data->shared.running = 0;  // Stop simulation
            pthread_mutex_unlock(&data->shared.print_lock);
            printf_messege(data, id, "DIED ❌");
            break;
        }

        printf_messege(data, id, "is sleeping");
        usleep(1000 * 1000); // 1s
    }
    return NULL;
}

int main(void)
{
    t_data data;
    pthread_t philo1, philo2;

    pthread_mutex_init(&data.shared.print_lock, NULL);
    data.shared.start_time = get_time();
    data.shared.running = 1;

    pthread_create(&philo1, NULL, philosopher, &data);
    pthread_create(&philo2, NULL, philosopher, &data);

    pthread_join(philo1, NULL);
    pthread_join(philo2, NULL);

    pthread_mutex_destroy(&data.shared.print_lock);
    return 0;
}
