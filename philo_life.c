#include "philo.h"

void philo_eat(t_philo *philo, t_data *data)
{
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(philo->right_fork);
        print_message(data, philo->id, MSG_FORK);
        pthread_mutex_lock(philo->left_fork);
        print_message(data, philo->id, MSG_FORK);
    }
    else
    {
        pthread_mutex_lock(philo->left_fork);
        print_message(data, philo->id, MSG_FORK);
        pthread_mutex_lock(philo->right_fork);
        print_message(data, philo->id, MSG_FORK);
    }

    pthread_mutex_lock(&philo->lock);
    philo->state = EATING;
    philo->last_meal_time = get_time();
    print_message(data, philo->id, MSG_EAT);
    pthread_mutex_unlock(&philo->lock);
    
    
    safe_sleep(data->shared.time_to_eat);
    
    pthread_mutex_lock(&philo->lock);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->lock);
    
    if (philo->id % 2 == 0)
    {
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
    }
    else
    {
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);
    }
}

void philo_sleep(t_philo *philo, t_data *data)
{
    pthread_mutex_lock(&philo->lock);
    philo->state = SLEEPING;
    pthread_mutex_unlock(&philo->lock);
    print_message(data, philo->id, MSG_SLEEP);
    safe_sleep(data->shared.time_to_sleep);
}

void philo_think(t_philo *philo, t_data *data)
{
    pthread_mutex_lock(&philo->lock);
    philo->state = THINKING;
    pthread_mutex_unlock(&philo->lock);
    print_message(data, philo->id, MSG_THINK);
    usleep(500);
}

int handle_one_philo(t_philo *philo, t_data *data)
{
    pthread_mutex_lock(philo->left_fork);
    print_message(data, philo->id, MSG_FORK);

    while (simulation_running(data))
        usleep(1000);
    pthread_mutex_unlock(philo->left_fork);
}

void *philosopher_life(void *philo_ptr)
{
    t_philo *philo;
    t_data  *data;

    philo = (t_philo *)philo_ptr;
    data = philo->data;
    if (philo->id % 2 == 0)
        usleep(1000);
        
    if (data->shared.num_philos == 1)
    {
        int result = handle_one_philo(philo, data);
        if (result != SUCCESS)
        {
            pthread_mutex_lock(&data->shared.print_lock);
            data->shared.running = 0;  
            pthread_mutex_unlock(&data->shared.print_lock);
        }
        
        return (NULL);
    }

    while (simulation_running(data))
    {
        philo_eat(philo, data);
        if (!simulation_running(data))
            break;
        philo_sleep(philo, data);
        if (!simulation_running(data))
            break;
        philo_think(philo, data);
    }
    return (NULL);
}