/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:10:22 by mtarza13          #+#    #+#             */
/*   Updated: 2025/04/10 22:10:48 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define TAKE_FORKS "has taken a fork"
# define THINK "is thinking"
# define SLEEP "is sleeping"
# define EAT "is eating"
# define DIED "died"

typedef enum e_philo_state
{
	EATING = 0,
	SLEEPING = 1,
	THINKING = 2,
	DEAD = 3,
	FULL = 4,
	DEFU = 5
}					t_state;

struct	s_data;

typedef struct s_philo
{
	int				id_philo;
	int				meals_count;
	long long		last_time_t_eat;

	struct s_data	*data;
	t_state			state;

	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	mut_state;
	pthread_mutex_t	mut_meals_count;
	pthread_mutex_t	mut_last_time_t_eat;

}					t_philo;

typedef struct s_data
{
	int				time_t_eat;
	int				time_t_sleep;
	int				time_t_die;
	long long		time_t_start;
	bool			is_active;

	int				nb_philos;
	int				nb_meals;

	pthread_mutex_t	mut_time_t_eat;
	pthread_mutex_t	mut_time_t_sleep;
	pthread_mutex_t	mut_time_t_die;
	pthread_mutex_t	mut_time_t_start;
	pthread_mutex_t	mut_print;
	pthread_mutex_t	mut_is_active;
	pthread_mutex_t	mut_nb_philos;
	pthread_mutex_t	*fork;

	t_philo			*philos;

	pthread_t		monit_death;
	pthread_t		monit_dining_complete;
	pthread_t		*philosopher_th;

}					t_data;

int					check_argment(int argc, char *argv[]);
int					check_is_digit(char *av[], int ac);
int					num_valid(int ac, char **av);
int					ft_atol(char *str);
int					ft_isdigit(int c);

int					init_data(t_data *data, int argc, char *argv[]);
void				init_default_data(t_data *data, int argc, char *argv[]);
int					allocate_resource(t_data *data);
int					init_mutex_all(t_data *data);

void				init_philo(t_data *data);
void				init_forks(t_data *data);
int					start_life(t_data *data);

void				*death_monitor(void *data_arg);
void				*dining_complete_monitor(void *data_p);

void				notify_all_philos(t_data *data);
int					philo_die(t_philo *philo);
t_state				get_state_philo(t_philo *philo);
int					get_die_time(t_philo *philo);

void				*handle_one_philo(t_philo *philo);
void				*routine(void *philo_arg);
int					dining_cycle(t_philo *philo);
int					eat(t_philo *philo);
int					calculate_thinking_time(t_philo *philo);

int					take_left_fork(t_philo *philo);
int					try_to_take_forks(t_philo *philo);
int					take_single_fork(t_philo *philo, pthread_mutex_t *fork);
void				get_fork_order(t_philo *philo, pthread_mutex_t **first,
						pthread_mutex_t **second);

bool				is_sim_running(t_data *data);
void				custom_sleep(int sleep_time, t_data *data);
void				update_meals_count(t_philo *philo);
void				print_status(t_philo *philo, char *msg);
long long			get_time(void);

void				set_last_time_eat(t_philo *philo, long long time);
void				set_philo_status(t_philo *philo, t_state state);
int					get_number_philos(t_data *data);
int					get_last_time_to_eat(t_philo *philo);

int					execute_philo(int ac, char *av[]);
void				join_threads(t_data *data);
void				cleanup_resources(t_data *data);

#endif
