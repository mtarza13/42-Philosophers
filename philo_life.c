#include "philo.h"

void philo_eat(t_philo *philo, t_data *data)
{
    pthread_mutex_lock(philo->left_fork);
    print_message(data, philo->id,MSG_FORK);
    pthread_mutex_lock(philo->right_fork);
    print_message(data, philo->id,MSG_FORK);

    pthread_mutex_lock(&philo->lock);
    philo->state = EATING ;
    philo->last_meal_time = get_time();
    printf_messge(data, philo->id,MSG_EAT);
    pthread_mutex_unlock(&philo->lock);

    /*eat for time_to_eat miliseconds */
    safe_sleep(data->shared.time_to_eat);

    /*updte meals eaten count*/
    pthread_mutex_lock(&philo->lock);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->lock);

    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_unlock(philo->left_fork);

}

void philo_sleep(t_philo *philo , t_data *data)
{
    pthread_mutex_lock(&philo->lock);
    philo->state = SLEEPING;
    pthread_mutex_unlock(&philo->lock);
    print_message(data,philo->id, MSG_SLEEP);
    safe_sleep(data->shared.time_to_sleep);
}

void philo_think(t_philo *philo, t_data *data)
{
    pthread_mutex_lock(&philo->lock);
    philo->state = THINKING;
    pthread_mutex_unlock(&philo->id);
    usleep(500);
}

void    handler_one_philo(t_philo *philo , t_data *data)
{
    pthread_mutex_lock(philo->left_fork);
    print_message(data, philo->id,MSG_FORK);

    while(simulation_running(data))
            usleep(1000);
    pthread_mutex_unlock(philo->left_fork);
}

void *philosopher_life(void *philo_ptr)
{
    t_philo *philo;
    t_data  *data;

    philo = (t_philo *)philo_ptr;
    data = philo->data;
    /* If odd numbrt in philosopher number */
    if(philo->id % 2 == 0)
        usleep(1000);
        if (data->shared.num_philos == 1)
        {
            handle_one_philo(philo,data);
            return(NULL);
        }
    while (simulation_running(data))
    {
            philo_eat(philo, data);
            philo_sleep(philo, data);
            philo_think(philo, data);
    }
}