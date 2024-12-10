/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 09:15:55 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/10 11:02:07 by llemmel          ###   ########.fr       */
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

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>

typedef struct s_arg
{
	int	nb_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	max_eat;
}	t_arg;

typedef struct s_fork
{
	pthread_mutex_t	mtx;
}	t_fork;

typedef struct s_philo_main	t_philo_main;
typedef struct s_philo
{
	pthread_t		th;
	t_fork			*right;
	t_fork			*left;
	t_philo_main	*philo_main;
}	t_philo;

typedef struct s_philo_main
{
	t_arg	arg;
	t_philo	*philos;
	t_fork	*forks;
}	t_philo_main;

/* UTILS */
int	ft_atoi_safe(const char *nptr);
int	print_error(char *msg, int ret_value);

#endif