/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:19:52 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/11 18:13:32 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	all_threads_ready(t_philo_main *philo_main)
{
	int	i;

	i = 0;
	while (i < philo_main->arg.nb_philo)
	{
		pthread_mutex_lock(&philo_main->philos[i].mtx);
		if (!philo_main->philos[i].is_ready)
		{
			printf("philo %d not ready\n", i);
			pthread_mutex_unlock(&philo_main->philos[i].mtx);
			return (0);
		}
		pthread_mutex_unlock(&philo_main->philos[i].mtx);
		i++;
	}
	return (1);
}

int	philos_dead(t_philo_main *philo_main)
{
	int	i;

	i = 0;
	while (i < philo_main->arg.nb_philo)
	{
		pthread_mutex_lock(&philo_main->philos[i].mtx);
		if (philo_main->philos[i].is_dead)
		{
			pthread_mutex_unlock(&philo_main->philos[i].mtx);
			return (1);
		}
		pthread_mutex_unlock(&philo_main->philos[i].mtx);
		i++;
	}
	return (1);
}

void	update_time(t_philo_main *philo_main)
{
	size_t	time;

	pthread_mutex_lock(&philo_main->mtx);
	time = philo_main->time;
	pthread_mutex_unlock(&philo_main->mtx);
	time = get_time_ms() - time;
	pthread_mutex_lock(&philo_main->mtx);
	philo_main->time += time;
	pthread_mutex_unlock(&philo_main->mtx);
}

int	eat_target(t_philo_main *philo_main)
{
	int	i;

	i = 0;
	while (i < philo_main->arg.nb_philo)
	{
		pthread_mutex_lock(&philo_main->philos[i].mtx);
		if (philo_main->philos[i].nb_eat < philo_main->arg.max_eat)
		{
			pthread_mutex_unlock(&philo_main->philos[i].mtx);
			return (0);
		}
		pthread_mutex_unlock(&philo_main->philos[i].mtx);
		i++;
	}
	return (1);
}

void	*monitoring_routine(void *arg)
{
	t_monitoring	*monitoring;

	monitoring = (t_monitoring *)arg;
	while (!all_threads_ready(monitoring->philo_main))
		continue ;
	// {
	// 	if (monitoring->philo_main->time >= MAX_INIT_TIME)
	// 	{
	// 		pthread_mutex_lock(&monitoring->philo_main->mtx);
	// 		monitoring->philo_main->error = 1;
	// 		pthread_mutex_unlock(&monitoring->philo_main->mtx);
	// 		return (NULL);
	// 	}
	// }
	pthread_mutex_lock(&monitoring->philo_main->mtx);
	monitoring->philo_main->is_running = 1;
	pthread_mutex_unlock(&monitoring->philo_main->mtx);
	while (1)
	{
		if (philos_dead(monitoring->philo_main))
		{
			pthread_mutex_lock(&monitoring->philo_main->mtx);
			monitoring->is_running = 0;
			pthread_mutex_unlock(&monitoring->philo_main->mtx);
		}
		else if (monitoring->philo_main->arg.max_eat != -1 \
			&& eat_target(monitoring->philo_main))
		{
			pthread_mutex_lock(&monitoring->philo_main->mtx);
			monitoring->is_running = 0;
			pthread_mutex_unlock(&monitoring->philo_main->mtx);
		}
	}
	return (NULL);
}

int	start_monitoring(t_philo_main *philo_main)
{
	memset(&philo_main->monitoring, 0, sizeof(t_monitoring));
	philo_main->monitoring.philo_main = philo_main;
	if (pthread_create(&philo_main->monitoring.th, NULL, &monitoring_routine, \
		&philo_main->monitoring) != 0)
	{
		pthread_mutex_lock(&philo_main->mtx);
		philo_main->error = 1;
		pthread_mutex_unlock(&philo_main->mtx);
		return (print_error(THREAD_ERROR, 0));
	}
	return (1);
}
