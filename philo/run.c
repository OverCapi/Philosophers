/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 13:03:04 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/09 17:38:20 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *arg)
{
	t_philo_child	*philo;

	philo = (t_philo_child *)arg;
	printf("index of thread : %d, current state : %d\n", philo->index, philo->state);
	return (NULL);
}

unsigned long int	get_deltatime(struct timeval initial_time)
{
	struct timeval		new_time;
	unsigned long int	delta_time;

	gettimeofday(&new_time, NULL);
	delta_time = (new_time.tv_sec - initial_time.tv_sec) * 1000000 + \
		new_time.tv_usec - initial_time.tv_usec;
	return (delta_time);
}

int	run(t_philo_main *philo_main)
{
	struct timeval	initial_time;

	gettimeofday(&initial_time, NULL);
	while (1)
	{
		pthread_mutex_lock(&philo_main->svar.m_running);
		if (!philo_main->svar.running)
			break ;
		pthread_mutex_unlock(&philo_main->svar.m_running);
		philo_main->time = get_deltatime(initial_time);
		// printf("time : %ld\n", philo_main->time);
		//usleep(100000);
	}
	pthread_mutex_unlock(&philo_main->svar.m_running);
	return (1);
}
