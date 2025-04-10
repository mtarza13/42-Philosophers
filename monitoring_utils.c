/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 22:06:52 by mtarza            #+#    #+#             */
/*   Updated: 2025/04/10 22:06:53 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	notify_all_philos(t_data *data)
{
	t_philo	*philos;
	int		i;
	int		nb_philos;

	nb_philos = get_number_philos(data);
	philos = data->philos;
	i = 0;
	while (i < nb_philos)
	{
		set_philo_status(&philos[i], DEAD);
		i++;
	}
}

int	philo_die(t_philo *philo)
{
	t_data		*data;
	long long	last_eat_time;

	data = philo->data;
	last_eat_time = get_last_time_to_eat(philo);
	if (get_time() - last_eat_time > data->time_t_die
		&& get_state_philo(philo) != DEAD)
	{
		set_philo_status(philo, DEAD);
		return (true);
	}
	return (false);
}

t_state	get_state_philo(t_philo *philo)
{
	t_state	state;

	pthread_mutex_lock(&philo->mut_state);
	state = philo->state;
	pthread_mutex_unlock(&philo->mut_state);
	return (state);
}

int	get_die_time(t_philo *philo)
{
	int	time;

	pthread_mutex_lock(&philo->data->mut_time_t_die);
	time = philo->data->time_t_die;
	pthread_mutex_unlock(&philo->data->mut_time_t_die);
	return (time);
}
