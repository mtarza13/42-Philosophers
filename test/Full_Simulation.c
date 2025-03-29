#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define MSG_FORK "has taken a fork 🍴"

typedef struct s_shared {
    pthread_mutex_t print_lock;
    long start_time;
    int running;
} t_shared;

typedef struct s_data {
    t_shared shared;
} t_data;

typedef struct s_philo {
    int id;
    pthread_mutex_t *left_fork;
    t_data *data; // Add this to store reference to shared data
} t_philo;

// Get current time in milliseconds
long get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

// Print message (thread-safe)
void print_message(t_data *data, int philo_id, char *message)
{
    long timestamp;
    pthread_mutex_lock(&data->shared.print_lock);
    if (data->shared.running)
    {
        timestamp = get_time() - data->shared.start_time;
        printf("%ld | Philosopher %d | %s\n", timestamp, philo_id, message);
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

// Handler function for one philosopher
void handler_one_philo(t_philo *philo)
{
    pthread_mutex_lock(philo->left_fork);
    print_message(philo->data, philo->id, MSG_FORK);

    while (simulation_running(philo->data)) 
    {
        usleep(1000);  // Philosopher waits for simulation to stop
    }

    // Add a message when philosopher gives up waiting
    if (!simulation_running(philo->data))
    {
        print_message(philo->data, philo->id, "Simulation stopped. Philosopher cannot eat.");
    }

    pthread_mutex_unlock(philo->left_fork);
}

// Philosopher thread function
void *philosopher(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    handler_one_philo(philo);
    return NULL;
}

int main(void)
{
    t_data data;
    t_philo philo;
    pthread_t philo_thread;
    pthread_mutex_t fork;

    pthread_mutex_init(&data.shared.print_lock, NULL);
    pthread_mutex_init(&fork, NULL);

    data.shared.start_time = get_time();
    data.shared.running = 1;  // Allow simulation to run

    philo.id = 1;             // Philosopher 1
    philo.left_fork = &fork;  // Single fork
    philo.data = &data;       // Pass reference to shared data

    pthread_create(&philo_thread, NULL, philosopher, &philo);

    usleep(5000 * 1000);  // Let the simulation run for 5 seconds

    // Stop simulation after 5 seconds
    pthread_mutex_lock(&data.shared.print_lock);
    data.shared.running = 0;  // Stop simulation
    pthread_mutex_unlock(&data.shared.print_lock);

    // Allow some time for the philosopher thread to finish
    usleep(500 * 1000);

    pthread_join(philo_thread, NULL);

    pthread_mutex_destroy(&fork);
    pthread_mutex_destroy(&data.shared.print_lock);

    return 0;
}
