/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 09:15:55 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/11 18:47:30 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define MAX_INIT_TIME 10000000 // 10s in µs

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

# define EAT_STATUS "is eating"
# define SLEEP_STATUS "is sleeping"
# define THINK_STATUS "is thinking"
# define FORK_STATUS "has taken a fork"
# define DIED_STATUS "died"

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>
# include <stdlib.h>

typedef struct s_arg
{
	size_t	nb_philo;
	size_t	time_to_die;
	size_t	time_to_eat;
	size_t	time_to_sleep;
	int		max_eat;
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
	int				last_time_eat;
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
	pthread_mutex_t	mtx;
}	t_philo_main;

/* UTILS */
int	ft_atoi_safe(const char *nptr);
int	print_error(char *msg, int ret_value);
int	get_time_ms(void);

/* INIT */
int	init(t_philo_main *philo_main);

int	print_status(t_philo *philo, char *status);
int	start_monitoring(t_philo_main *philo_main);
int	run(t_philo_main *philo_main);

/* MUTEX */
int	set_int_mutex(pthread_mutex_t *mtx, int *ptr, int value);
int	get_int_mutex(pthread_mutex_t *mtx, int *ptr, int *err);

#endif