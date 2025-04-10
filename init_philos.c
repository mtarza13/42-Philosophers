/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 22:06:46 by mtarza            #+#    #+#             */
/*   Updated: 2025/04/10 22:09:01 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_forks(t_data *data)
{
	int		i;
	t_philo	*philo;

	i = 0;
	philo = data->philos;
	while (i < data->nb_philos)
	{
		pthread_mutex_init(&data->fork[i], NULL);
		i++;
	}
	i = 0;
	philo[0].left_fork = &data->fork[0];
	philo[0].right_fork = &data->fork[data->nb_philos - 1];
	while (++i < data->nb_philos)
	{
		philo[i].left_fork = &data->fork[i];
		philo[i].right_fork = &data->fork[i - 1];
	}
}

void	init_philo(t_data *data)
{
	int		i;
	t_philo	*current_philo;
	t_philo	*philo;

	i = 0;
	philo = data->philos;
	while (i < data->nb_philos)
	{
		current_philo = &philo[i];
		current_philo->data = data;
		current_philo->state = DEFU;
		current_philo->meals_count = 0;
		current_philo->id_philo = i;
		pthread_mutex_init(&current_philo->mut_state, NULL);
		pthread_mutex_init(&current_philo->mut_meals_count, NULL);
		pthread_mutex_init(&current_philo->mut_last_time_t_eat, NULL);
		i++;
	}
}

int	create_philosopher_threads(t_data *data)
{
	int	i;
	int	nb_philo;

	nb_philo = get_number_philos(data);
	data->time_t_start = get_time();
	i = 0;
	while (i < nb_philo)
	{
		if (pthread_create(&data->philosopher_th[i], NULL, &routine,
				&data->philos[i]) != 0)
			return (printf("Error creating philosopher thread\n"), 1);
		i++;
	}
	return (0);
}

int	create_monitor_threads(t_data *data)
{
	usleep(100);
	if (pthread_create(&data->monit_death, NULL, &death_monitor, data) != 0)
		return (printf("Error creating monitor thread\n"), 1);
	pthread_mutex_lock(&data->mut_nb_philos);
	if (data->nb_meals > 0)
	{
		if (pthread_create(&data->monit_dining_complete, NULL,
				&dining_complete_monitor, data) != 0)
		{
			pthread_mutex_unlock(&data->mut_nb_philos);
			return (printf("Error creating meals monitor thread\n"), 1);
		}
	}
	pthread_mutex_unlock(&data->mut_nb_philos);
	return (0);
}

int	start_life(t_data *data)
{
	if (create_philosopher_threads(data) != 0)
		return (1);
	if (create_monitor_threads(data) != 0)
		return (1);
	return (0);
}
