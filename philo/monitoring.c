/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:19:52 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/14 15:31:14 by llemmel          ###   ########.fr       */
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
			pthread_mutex_unlock(&philo_main->philos[i].mtx);
			return (0);
		}
		pthread_mutex_unlock(&philo_main->philos[i].mtx);
		i++;
	}
	return (1);
}

int	is_dead(t_philo *philo)
{
	long int	current_time;

	pthread_mutex_lock(&philo->philo_main->mtx);
	current_time = philo->philo_main->time;
	pthread_mutex_unlock(&philo->philo_main->mtx);
	pthread_mutex_lock(&philo->mtx);
	if (current_time - philo->last_time_eat >= philo->philo_main->arg.time_to_die + 5) // +5 for safety
	{
		philo->is_dead = 1;
		pthread_mutex_unlock(&philo->mtx);
		return (1);
	}
	pthread_mutex_unlock(&philo->mtx);
	return (0);
}

int	philos_dead(t_philo_main *philo_main)
{
	int	i;

	i = 0;
	while (i < philo_main->arg.nb_philo)
	{
		if (is_dead(&philo_main->philos[i]))
			return (1);
		i++;
	}
	return (0);
}

void	update_time(t_philo_main *philo_main)
{
	long int	current_time;

	current_time = get_time_ms(philo_main);
	pthread_mutex_lock(&philo_main->mtx);
	philo_main->time = current_time;
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
	pthread_mutex_lock(&monitoring->philo_main->mtx);
	monitoring->philo_main->is_running = 1;
	pthread_mutex_unlock(&monitoring->philo_main->mtx);
	while (1)
	{
		update_time(monitoring->philo_main);
		if (philos_dead(monitoring->philo_main))
		{
			pthread_mutex_lock(&monitoring->philo_main->mtx);
			monitoring->philo_main->is_running = 0;
			pthread_mutex_unlock(&monitoring->philo_main->mtx);
			break ;
		}
		if (monitoring->philo_main->arg.max_eat != -1 \
			&& eat_target(monitoring->philo_main))
		{
			pthread_mutex_lock(&monitoring->philo_main->mtx);
			monitoring->is_running = 0;
			pthread_mutex_unlock(&monitoring->philo_main->mtx);
			break ;
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
