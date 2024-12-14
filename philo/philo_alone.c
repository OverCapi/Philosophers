/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_alone.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 14:57:40 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/14 15:59:19 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*alone_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->mtx);
	philo->is_ready = 1;
	pthread_mutex_unlock(&philo->mtx);
	while (1)
	{
		pthread_mutex_lock(&philo->philo_main->mtx);
		if (philo->philo_main->is_running)
		{
			pthread_mutex_unlock(&philo->philo_main->mtx);
			break ;
		}
		pthread_mutex_unlock(&philo->philo_main->mtx);
	}
	if (!print_status(philo, FORK_STATUS))
		return (NULL);
	while (!is_finished(philo))
		continue ;
	return (NULL);
}

int	philo_alone(t_philo_main *philo_main)
{
	if (pthread_create(&philo_main->philos[0].th, NULL, &alone_routine, \
		&philo_main->philos[0]) != 0)
	{
		pthread_mutex_lock(&philo_main->mtx);
		philo_main->error = 1;
		pthread_mutex_unlock(&philo_main->mtx);
		return (print_error(THREAD_ERROR, 0));
	}
	start_monitoring(philo_main);
	if (pthread_join(philo_main->monitoring.th, NULL) != 0)
	{
		pthread_mutex_lock(&philo_main->mtx);
		philo_main->error = 1;
		pthread_mutex_unlock(&philo_main->mtx);
		return (print_error(JOIN_ERROR, 0));
	}
	return (1);
}
