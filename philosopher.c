/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 22:07:04 by mtarza            #+#    #+#             */
/*   Updated: 2025/04/10 22:07:05 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*handle_one_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, TAKE_FORKS);
	set_philo_status(philo, EATING);
	custom_sleep(philo->data->time_t_die, philo->data);
	return (NULL);
}

void	*routine(void *philo_arg)
{
	int		nb_philo;
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)philo_arg;
	data = philo->data;
	nb_philo = get_number_philos(data);
	set_last_time_eat(philo, get_time());
	if (nb_philo == 1)
		return (handle_one_philo(philo));
	if (philo->id_philo % 2 != 0)
		usleep(data->time_t_eat * 500);
	while (is_sim_running(data))
	{
		if (dining_cycle(philo) != 0)
			break ;
	}
	return (NULL);
}

int	dining_cycle(t_philo *philo)
{
	t_data	*data;
	int		think_time;

	data = philo->data;
	if (eat(philo) != 0)
		return (1);
	if (is_sim_running(data))
	{
		set_philo_status(philo, SLEEPING);
		print_status(philo, SLEEP);
		custom_sleep(data->time_t_sleep, data);
	}
	if (is_sim_running(data))
	{
		set_philo_status(philo, THINKING);
		print_status(philo, THINK);
		think_time = calculate_thinking_time(philo);
		if (think_time > 0)
			custom_sleep(think_time, data);
	}
	return (0);
}

int	eat(t_philo *philo)
{
	if (try_to_take_forks(philo) != 0)
		return (1);
	set_philo_status(philo, EATING);
	set_last_time_eat(philo, get_time());
	print_status(philo, EAT);
	custom_sleep(philo->data->time_t_eat, philo->data);
	update_meals_count(philo);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	return (0);
}

int	calculate_thinking_time(t_philo *philo)
{
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	thinking_time;

	time_to_die = philo->data->time_t_die;
	time_to_eat = philo->data->time_t_eat;
	time_to_sleep = philo->data->time_t_sleep;
	if (time_to_eat + time_to_sleep >= time_to_die - 20)
		return (0);
	thinking_time = time_to_die - time_to_eat - time_to_sleep - 20;
	if (thinking_time > 200)
		thinking_time = 200;
	if (philo->id_philo % 2 == 0)
		thinking_time = thinking_time / 2;
	if (thinking_time < 0)
		thinking_time = 0;
	return (thinking_time);
}
