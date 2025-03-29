#include "philo.h"

long get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void    safe_sleep(int milliseconds)
{
    long start;
    long current;
    start = get_time();
    while(1)
    {
    current = get_time();
    if(current - start >= milliseconds)
        break;
        usleep(100);
    }
}

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

int simulation_running(t_data *data)
{
    int status;

    pthread_mutex_lock(&data->shared.print_lock);
    status = data->shared.running;
    pthread_mutex_unlock(&data->shared.print_lock);
    return(status);
}
