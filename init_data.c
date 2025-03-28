#include "philo.h"

int init_forks(t_shared *data)
{
	int i ;
	i = 0;

	while(i < data->num_philos)
	{
		if(pthread_mutex_init(&data->forks[i],NULL) != NULL)
		{
			while(i--)
				pthread_mutex_destroy(&data->forks[i]);
				free(data->forks);
				return(EXIT_FAILURE);
		}
		if(pthread_mutex_init(&data->print_lock ,NULL) != NULL)
		{
			i = 0;
			while(i--)
			pthread_mutex_destroy(&data->forks[i]);
			free(data->forks);
			return(EXIT_FAILURE);
		}
	}
	return(SUCCESS);
}
int init_shared(t_shared *shared , int argc, char *argv[])
{
	shared->num_philos = ft_atol(argv[1]);
	shared->time_to_die = ft_atol(argv[2]);
	shared->time_to_eat = ft_atol(argv[3]);
	shared->time_to_sleep = ft_atol(argv[4]);
	shared->meals_needed = -1;
	if(argc == 6)
		shared->meals_needed = ft_atol(argv[5]);

		shared->running = 1;
		shared->start_time = 0;
	shared->forks = malloc(shared->num_philos *  sizeof(pthread_mutex_t));
	if(shared->forks)
		return(FAILURE);

	if(init_forks(&shared ) != SUCCESS)
		return(FAILURE);
}

int init_philosophers(t_data *data)
{
	int i = 0;
	t_shared	*shared;

	shared = &data->shared;
	data->philosophers = malloc(shared->num_philos * sizeof(t_philo));
	if(data)
}

int init_program(t_data *data , int argc , char *argv[])
{
	int i ;
	i = 0;
	if(init_shared(&data->shared ,argc,argv)	!= SUCCESS)
	{
		ft_putsrt("Failer to initialize shared resource\n");
		return(FAILURE);
	}
	if(init_philosophers(data) != SUCCESS)
	{
		while(i < data->shared.num_philos)
			pthread_mutex_destroy(&data->shared.forks[i]);
		pthread_mutex_destroy(&data->shared.forks);
		
		free(data->shared.forks);
		ft_putstr("Failed to initialize philosophers\n");
		return (FAILURE);
	}
}