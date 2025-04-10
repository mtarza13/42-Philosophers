/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 22:06:38 by mtarza            #+#    #+#             */
/*   Updated: 2025/04/10 22:06:39 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	try_to_take_forks(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	get_fork_order(philo, &first_fork, &second_fork);
	if (take_single_fork(philo, first_fork) != 0)
		return (1);
	pthread_mutex_lock(second_fork);
	if (!is_sim_running(philo->data))
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return (1);
	}
	print_status(philo, TAKE_FORKS);
	return (0);
}

int	take_single_fork(t_philo *philo, pthread_mutex_t *fork)
{
	pthread_mutex_lock(fork);
	if (!is_sim_running(philo->data))
	{
		pthread_mutex_unlock(fork);
		return (1);
	}
	print_status(philo, TAKE_FORKS);
	return (0);
}

void	get_fork_order(t_philo *philo, pthread_mutex_t **first,
		pthread_mutex_t **second)
{
	if (philo->id_philo % 2 == 0)
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
	else
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
}
