/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 22:06:54 by mtarza            #+#    #+#             */
/*   Updated: 2025/04/10 22:20:05 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_philosopher_death(t_philo *philos, t_data *data, int i)
{
	long long	last_meal_time;
	long long	current_time;

	current_time = get_time();
	pthread_mutex_lock(&philos[i].mut_last_time_t_eat);
	last_meal_time = philos[i].last_time_t_eat;
	pthread_mutex_unlock(&philos[i].mut_last_time_t_eat);
	if (current_time - last_meal_time > data->time_t_die)
	{
		pthread_mutex_lock(&data->mut_print);
		if (is_sim_running(data))
		{
			printf("%lld %d died\n", last_meal_time + data->time_t_die
				- data->time_t_start, philos[i].id_philo + 1);
			pthread_mutex_lock(&data->mut_is_active);
			data->is_active = false;
			pthread_mutex_unlock(&data->mut_is_active);
		}
		pthread_mutex_unlock(&data->mut_print);
		return (1);
	}
	return (0);
}

void	*death_monitor(void *data_arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)data_arg;
	while (is_sim_running(data))
	{
		i = 0;
		while (i < data->nb_philos && is_sim_running(data))
		{
			if (check_philosopher_death(data->philos, data, i))
				return (NULL);
			i++;
			usleep(10000);
		}
	}
	return (NULL);
}

static bool	are_all_full(t_data *data)
{
	int		i;
	int		meal_count;
	bool	all_full;

	all_full = true;
	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_lock(&data->philos[i].mut_meals_count);
		meal_count = data->philos[i].meals_count;
		pthread_mutex_unlock(&data->philos[i].mut_meals_count);
		if (meal_count < data->nb_meals)
		{
			all_full = false;
			break ;
		}
		i++;
	}
	return (all_full);
}

void	*dining_complete_monitor(void *data_p)
{
	t_data	*data;

	data = (t_data *)data_p;
	while (is_sim_running(data))
	{
		if (are_all_full(data))
		{
			pthread_mutex_lock(&data->mut_print);
			printf("%lld All philosophers have eaten enough\n", get_time()
				- data->time_t_start);
			pthread_mutex_unlock(&data->mut_print);
			pthread_mutex_lock(&data->mut_is_active);
			data->is_active = false;
			pthread_mutex_unlock(&data->mut_is_active);
			notify_all_philos(data);
			break ;
		}
	}
	return (NULL);
}
