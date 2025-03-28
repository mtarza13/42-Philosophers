#include "philo.h"
 
/*check if every philo have eating in enough in time */

int     all_have_eaten_enough(t_data *data)
{
    int i ;
    int finishs;
    if(data->shared.meals_needed == -1)
        return(1);
    i = 0;
    finishs =1 ;
    while(i < data->shared.num_philos)
    {
        pthread_mutex_lock(&data->philosophers);
      if  (data->philosophers[i].meals_eaten < data->shared.meals_needed)    
          {
            finishs = 0;
            pthread_mutex_unlock(&data->philosophers[i].lock);
            break;
        }
        pthread_mutex_unlock(&data->philosophers[i].lock);
        i++;
    }
    return(finishs);
}
int check_starvation(t_data *data)
{
    int i;
    long now;
    long    time_since_last_meal;

    i = 0;
    now = get_time();

    while(i < data->shared.num_philos)
    {
        pthread_mutex_lock(&data->philosophers[i].lock);

        time_since_last_meal = now - data->philosophers[i].last_meal_time;
        if(data->philosophers[i].state != EATING && time_since_last_meal > data->shared.time_to_die )
        {
            pthread_mutex_unlock(&data->philosophers[i].lock);
            
            pthread_mutex_lock(&data->shared.print_lock);
            printf("%ld %d %s\n", now - data->shared.start_time,
				data->philosophers[i].id, MSG_DIED);
                data->shared.running = 0;
            pthread_mutex_unlock(&data->shared.print_lock);
            return(1);
        }
        pthread_mutex_unlock(&data->philosophers[i].lock);
        i++;
    }

    return(0);
}


void monitor_philosophers(t_data *data)
{
    while(1)
    {
        if(check_starvation(data) || all_have_eaten_enough(data))
        {
            pthread_mutex_lock(&data->shared.print_lock);
            data->shared.running = 0;
            pthread_mutex_unlock(data);
            break;
        }
        usleep(1000);
    }
}
void start_simulation(t_data *data)
{
    int i ;

    i = 0;
    /*record start  time*/
    data->shared.start_time = get_time;

    while(i < data->shared.num_philos)
    {
        pthread_mutex_lock(&data->philosophers[i].lock);
        data->philosophers[i].last_meal_time = data->shared.start_time;
        pthread_mutex_unlock(&data->philosophers[i].lock);
        i++;
    }
    i  = 0;
    while(i < data->shared.num_philos)
    {
        if(pthread_create(&data->threads[i],NULL,philosopher_life,&data->philosophers[i]) != 0)
        {
            ft_putstr("Error: Failed to create thread\n");
			pthread_mutex_lock(&data->shared.print_lock);
			data->shared.running = 0;
			pthread_mutex_unlock(&data->shared.print_lock);
			break;
        }
        i++;
    }
	monitor_philosophers(data);
    i = 0;
    while(i < data->shared.num_philos)
    {
        pthread_join(data->threads[i],NULL);
    i++;
    }
}