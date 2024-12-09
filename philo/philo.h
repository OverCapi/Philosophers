/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:20:28 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/09 17:38:41 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "utils/utils.h"

# include <pthread.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

# define ERROR_ARG "usage : ./philo \
number_of_philosophers \
time_to_die \
time_to_eat \
time_to_sleep \
[number_of_times_each_philosopher_must_eat]\n"
# define ERROR_ALLOC "memory alloction failed\n"
# define ERROR_INIT "Initialization failed\n"

typedef struct s_arg
{
	int	nb_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	nbt_philo_each_must_eat;
}	t_arg;

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				used;
}	t_fork;

typedef struct s_philo_main	t_philo_main;
// state : 0 : thinking, 1 : eating, 2 : sleepling, 3 : dead
typedef struct s_philo_child
{
	pthread_t			th;
	t_philo_main		*philo_main;
	int					index;
	int					state;
	int					nb_fork_use;
	unsigned long int	time_left;
	t_fork				*right;
	t_fork				*left;
}	t_philo_child;

typedef struct s_share_var
{
	pthread_mutex_t	m_running;
	int				running;
	t_fork			*fork;
	pthread_mutex_t	m_nb_fork_used;
	int				nb_fork_used;
}	t_share_var;

typedef struct s_action
{
	int	action_type;
	int	is_pair;
}	t_action;

typedef struct s_philo_main
{
	t_arg				arg;
	t_philo_child		*philo;
	unsigned long int	time;
	t_share_var			svar;
	t_action			action;
}	t_philo_main;

/* INIT */
int		init_arg(t_arg *arg, int argc, char **argv);
int		init_philo_child(t_philo_main *philo_main);
int		init_philo_main(t_philo_main *philo_main);
int		join_philo(t_philo_main *philo_main);
int		destroy_mutex(t_philo_main *philo_main, int n);

/* RUN */
void	*routine(void *arg);
int		run(t_philo_main *philo_main);

#endif