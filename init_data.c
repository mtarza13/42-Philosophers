#include "philo.h"

int init_forks(t_shared *shared)
{
    int i;
    i = 0;

    while (i < shared->num_philos)
    {
        if (pthread_mutex_init(&shared->forks[i], NULL) != 0)
        {
            while (i--)
                pthread_mutex_destroy(&shared->forks[i]);
            free(shared->forks);
            return (FAILURE);
        }
        i++;
    }
    
    if (pthread_mutex_init(&shared->print_lock, NULL) != 0)
    {
        i = shared->num_philos;
        while (i--)
            pthread_mutex_destroy(&shared->forks[i]);
        free(shared->forks);
        return (FAILURE);
    }
    
    return (SUCCESS);
}

int init_shared(t_shared *shared, int argc, char *argv[])
{
    shared->num_philos = ft_atol(argv[1]);
    shared->time_to_die = ft_atol(argv[2]);
    shared->time_to_eat = ft_atol(argv[3]);
    shared->time_to_sleep = ft_atol(argv[4]);
    shared->meals_needed = -1;
    if (argc == 6)
        shared->meals_needed = ft_atol(argv[5]);

    shared->running = 1;
    shared->start_time = 0;
    shared->forks = malloc(shared->num_philos * sizeof(pthread_mutex_t));
    if (!shared->forks)
        return (FAILURE);

    if (init_forks(shared) != SUCCESS)
        return (FAILURE);
        
    return (SUCCESS);  
}

int init_thread(t_data *data, t_shared *shared)
{
    int i = 0;
    data->threads = malloc(shared->num_philos * sizeof(pthread_t));
    if (!data->threads)
    {
        while (i < shared->num_philos)
        {
            pthread_mutex_destroy(&data->philosophers[i].lock);
            i++;
        }
        free(data->philosophers);
        return (FAILURE);
    }
    return (SUCCESS);
}

int init_philosophers(t_data *data)
{
    int i = 0;
    t_shared *shared;

    shared = &data->shared;
    data->philosophers = malloc(shared->num_philos * sizeof(t_philo));
    if (!data->philosophers)  
        return (FAILURE);
        
    while (i < shared->num_philos)
    {
        data->philosophers[i].id = i + 1;
        data->philosophers[i].state = THINKING;
        data->philosophers[i].meals_eaten = 0;
        data->philosophers[i].last_meal_time = 0;
        data->philosophers[i].data = data;

        data->philosophers[i].left_fork = &shared->forks[i];
        data->philosophers[i].right_fork = &shared->forks[(i + 1) % shared->num_philos];

        if (pthread_mutex_init(&data->philosophers[i].lock, NULL) != 0)
        {
            while (--i >= 0)
                pthread_mutex_destroy(&data->philosophers[i].lock);
            free(data->philosophers);
            return (FAILURE);
        }
        i++;
    }
    
    if (init_thread(data, shared) != SUCCESS)  
        return (FAILURE);
        
    return (SUCCESS);
}

int init_program(t_data *data, int argc, char *argv[])
{
    int i;
    i = 0;
    if (init_shared(&data->shared, argc, argv) != SUCCESS)
    {
        ft_putstr("Failed to initialize shared resources\n");
        return (FAILURE);
    }

    if (init_philosophers(data) != SUCCESS)
    {
        while (i < data->shared.num_philos)
            pthread_mutex_destroy(&data->shared.forks[i++]);
            
        pthread_mutex_destroy(&data->shared.print_lock);
        free(data->shared.forks);
        ft_putstr("Failed to initialize philosophers\n");
        return (FAILURE);  
    }
    
    return (SUCCESS);  
}