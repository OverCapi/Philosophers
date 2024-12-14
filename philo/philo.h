/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 09:15:55 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/14 15:53:03 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define ARG_ERROR "Usage : ./philo number_of_philosophers \
time_to_die \
time_to_eat \
time_to_sleep \
[number_of_times_each_philosopher_must_eat]"
# define ARG_VALUE_ERROR "Parameters must be greater or equal than 0"
# define ALLOC_ERROR "Memory allocation failed"
# define INIT_ERROR "Initialization failed"
# define THREAD_ERROR "Thread creation failed"
# define JOIN_ERROR "Thread join failed"

# define EAT_STATUS "is eating\n"
# define SLEEP_STATUS "is sleeping\n"
# define THINK_STATUS "is thinking\n"
# define FORK_STATUS "has taken a fork\n"
# define DIED_STATUS "died\n"

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>
# include <stdlib.h>

typedef struct s_arg
{
	long int	nb_philo;
	long int	time_to_die;
	long int	time_to_eat;
	long int	time_to_sleep;
	long int	max_eat;
}	t_arg;

typedef struct s_fork
{
	pthread_mutex_t	mtx;
}	t_fork;

typedef struct s_philo_main	t_philo_main;
typedef struct s_philo
{
	pthread_t		th;
	int				index;
	pthread_mutex_t	mtx;
	int				is_dead;
	int				nb_eat;
	int				is_ready;
	long int		last_time_eat;
	t_fork			*right;
	t_fork			*left;
	t_philo_main	*philo_main;
}	t_philo;

typedef struct s_monitoring
{
	pthread_t		th;
	int				is_running;
	int				is_dead;
	int				nb_philo_reached_max_eat;
	t_philo_main	*philo_main;
}	t_monitoring;

typedef struct s_philo_main
{
	t_arg			arg;
	t_philo			*philos;
	t_fork			*forks;
	t_monitoring	monitoring;
	size_t			start_time;
	size_t			time;
	int				is_running;
	int				error;
	pthread_mutex_t	can_write;
	pthread_mutex_t	mtx;
}	t_philo_main;

/* UTILS */
int	ft_atoi_safe(const char *nptr);
int	print_error(char *msg, int ret_value);
long int	get_time_ms(t_philo_main *philo_main);
void	ft_usleep(long int time_us);

/* INIT */
int	init(t_philo_main *philo_main);

int	print_status(t_philo *philo, char *status);
int	start_monitoring(t_philo_main *philo_main);
int	run(t_philo_main *philo_main);
int	philo_alone(t_philo_main *philo_main);
int	is_finished(t_philo *philo);
int	philo_even(t_philo_main *philo_main);

/* MUTEX */
int	set_int_mutex(pthread_mutex_t *mtx, int *ptr, int value);
int	get_int_mutex(pthread_mutex_t *mtx, int *ptr, int *err);

#endif