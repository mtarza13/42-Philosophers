/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 22:07:08 by mtarza            #+#    #+#             */
/*   Updated: 2025/04/10 22:07:09 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_last_time_eat(t_philo *philo, long long time)
{
	pthread_mutex_lock(&philo->mut_last_time_t_eat);
	philo->last_time_t_eat = time;
	pthread_mutex_unlock(&philo->mut_last_time_t_eat);
}

void	set_philo_status(t_philo *philo, t_state state)
{
	pthread_mutex_lock(&philo->mut_state);
	philo->state = state;
	pthread_mutex_unlock(&philo->mut_state);
}

int	get_number_philos(t_data *data)
{
	int	nb_philo;

	pthread_mutex_lock(&data->mut_nb_philos);
	nb_philo = data->nb_philos;
	pthread_mutex_unlock(&data->mut_nb_philos);
	return (nb_philo);
}

int	get_last_time_to_eat(t_philo *philo)
{
	int	last_time;

	pthread_mutex_lock(&philo->mut_last_time_t_eat);
	last_time = philo->last_time_t_eat;
	pthread_mutex_unlock(&philo->mut_last_time_t_eat);
	return (last_time);
}

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
