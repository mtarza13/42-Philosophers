/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 22:06:48 by mtarza            #+#    #+#             */
/*   Updated: 2025/04/10 22:06:49 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_mutex_all(t_data *data)
{
	if (pthread_mutex_init(&data->mut_time_t_eat, NULL)
		|| pthread_mutex_init(&data->mut_time_t_sleep, NULL)
		|| pthread_mutex_init(&data->mut_time_t_die, NULL)
		|| pthread_mutex_init(&data->mut_time_t_start, NULL)
		|| pthread_mutex_init(&data->mut_print, NULL)
		|| pthread_mutex_init(&data->mut_is_active, NULL)
		|| pthread_mutex_init(&data->mut_nb_philos, NULL))
		return (1);
	return (0);
}

int	allocate_resource(t_data *data)
{
	if (!data)
		return (1);
	data->philos = malloc(sizeof(t_philo) * data->nb_philos);
	data->fork = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	data->philosopher_th = malloc(sizeof(pthread_t) * data->nb_philos);
	if (!data->philos || !data->fork || !data->philosopher_th)
	{
		if (data->philos)
			free(data->philos);
		if (data->fork)
			free(data->fork);
		if (data->philosopher_th)
			free(data->philosopher_th);
		return (1);
	}
	return (0);
}

void	init_default_data(t_data *data, int argc, char *argv[])
{
	data->is_active = true;
	data->nb_philos = ft_atol(argv[1]);
	data->time_t_die = ft_atol(argv[2]);
	data->time_t_eat = ft_atol(argv[3]);
	data->time_t_sleep = ft_atol(argv[4]);
	data->nb_meals = -1;
	if (argc == 6)
	{
		data->nb_meals = ft_atol(argv[5]);
	}
}

int	init_data(t_data *data, int argc, char *argv[])
{
	init_default_data(data, argc, argv);
	if (init_mutex_all(data) != 0)
		return (1);
	if (allocate_resource(data) != 0)
		return (1);
	return (0);
}
