/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 22:06:50 by mtarza            #+#    #+#             */
/*   Updated: 2025/04/10 22:08:22 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	execute_philo(int ac, char *av[])
{
	t_data	data;

	if (init_data(&data, ac, av) != 0)
		return (1);
	init_philo(&data);
	init_forks(&data);
	if (start_life(&data) != 0)
		return (1);
	join_threads(&data);
	cleanup_resources(&data);
	return (0);
}

void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_join(data->philosopher_th[i], NULL);
		i++;
	}
	pthread_join(data->monit_death, NULL);
	if (data->nb_meals > 0)
		pthread_join(data->monit_dining_complete, NULL);
}

void	cleanup_resources(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_destroy(&data->fork[i]);
		i++;
	}
	pthread_mutex_destroy(&data->mut_time_t_eat);
	pthread_mutex_destroy(&data->mut_time_t_sleep);
	pthread_mutex_destroy(&data->mut_time_t_die);
	pthread_mutex_destroy(&data->mut_time_t_start);
	pthread_mutex_destroy(&data->mut_print);
	pthread_mutex_destroy(&data->mut_is_active);
	pthread_mutex_destroy(&data->mut_nb_philos);
	free(data->fork);
	free(data->philos);
	free(data->philosopher_th);
}

int	main(int argc, char *argv[])
{
	if (check_argment(argc, argv) != 0)
		return (1);
	if (execute_philo(argc, argv) != 0)
		return (1);
	return (0);
}
