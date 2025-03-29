/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza13 <mtarza13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-29 01:50:36 by mtarza13          #+#    #+#             */
/*   Updated: 2025-03-29 01:50:36 by mtarza13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

# define SUCCESS 0
# define FAILURE 1
# define EXIT_FAILURE 1

# define MSG_DIED "died"
# define MSG_EAT "is eating"
# define MSG_SLEEP "is sleeping"
# define MSG_THINK "is thinking"
# define MSG_FORK "has taken a fork"

typedef enum e_state
{
	THINKING,
	EATING,
	SLEEPING,
	DEAD
}	t_state;

typedef struct s_data	t_data;
typedef struct s_shared	t_shared;
typedef struct s_philo	t_philo;

typedef struct s_philo
{
	int				id;
	t_state			state;
	int				meals_eaten;
	long			last_meal_time;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
	pthread_mutex_t	lock;
}	t_philo;

typedef struct s_shared
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_needed;
	pthread_mutex_t	*forks;
	int				running;
	long			start_time;
	pthread_mutex_t	print_lock;
}	t_shared;

typedef struct s_data
{
	t_philo			*philosophers;
	t_shared		shared;
	pthread_t		*threads;
}	t_data;

int		init_forks(t_shared *shared);
int		init_shared(t_shared *shared, int argc, char *argv[]);
int		init_thread(t_data *data, t_shared *shared);
int		init_philosophers(t_data *data);
int		init_program(t_data *data, int argc, char *argv[]);

int		all_have_eaten_enough(t_data *data);
int		check_starvation(t_data *data);
void	monitor_philosophers(t_data *data);
void	start_simulation(t_data *data);
void	clean_up(t_data *data);

void philo_eat(t_philo *philo, t_data *data);
void	philo_sleep(t_philo *philo, t_data *data);
void	philo_think(t_philo *philo, t_data *data);
void	handle_one_philo(t_philo *philo, t_data *data);
void	*philosopher_life(void *philo_ptr);

long	get_time(void);
void	safe_sleep(int milliseconds);
void	print_message(t_data *data, int philo_id, char *message);
int		simulation_running(t_data *data);

int		check_is_digit(char *av[], int ac);
int		check_num(int ac, char **av);
int		ft_atol(char *str);
void	ft_putstr(char *s);
int		ft_isdigit(int c);

#endif