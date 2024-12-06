/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:20:28 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/06 14:40:10 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "utils/utils.h"

# include <pthread.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>

# define ERROR_ARG "usage : ./philo \
number_of_philosophers \
time_to_die \
time_to_eat \
time_to_sleep \
[number_of_times_each_philosopher_must_eat]\n"
# define ERROR_ALLOC "memory alloction failed\n"

typedef struct s_arg
{
	int	nb_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	nbt_philo_each_must_eat;
}	t_arg;

typedef struct s_philo_main
{
	t_arg 		arg;
	pthread_t	*philo;
}	t_philo_main;

#endif