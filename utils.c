/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 22:07:07 by mtarza            #+#    #+#             */
/*   Updated: 2025/04/10 22:07:08 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_sim_running(t_data *data)
{
	bool	result;

	pthread_mutex_lock(&data->mut_is_active);
	result = data->is_active;
	pthread_mutex_unlock(&data->mut_is_active);
	return (result);
}

void	custom_sleep(int sleep_time, t_data *data)
{
	long long	start;

	start = get_time();
	while (is_sim_running(data) && (get_time() - start) < sleep_time)
		usleep(100);
}

void	update_meals_count(t_philo *philo)
{
	pthread_mutex_lock(&philo->mut_meals_count);
	philo->meals_count++;
	pthread_mutex_unlock(&philo->mut_meals_count);
}

void	print_status(t_philo *philo, char *msg)
{
	t_data	*data;

	data = philo->data;
	if (!is_sim_running(data))
		return ;
	pthread_mutex_lock(&data->mut_print);
	if (is_sim_running(data))
		printf("%lld %d %s\n", get_time() - data->time_t_start, philo->id_philo
			+ 1, msg);
	pthread_mutex_unlock(&data->mut_print);
}
