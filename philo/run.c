/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:31:14 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/11 18:48:21 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_time_ms(t_philo *philo)
{
	struct timeval	time;
	size_t			current_time;

	gettimeofday(&time, NULL);
	current_time = time.tv_sec * 1000 + time.tv_usec / 1000;
	if (philo->philo_main->start_time == 0)
		philo->philo_main->start_time = current_time;
	return (current_time - philo->philo_main->start_time);
}

int	eat_target_reached(t_philo *philo)
{
	return (philo->nb_eat >= philo->philo_main->arg.max_eat);
}

int	philo_dead(t_philo *philo)
{
	printf("philo %d : philo_dead : %ld, (%ld)\n", philo->index,get_time_ms() - philo->last_time_eat, philo->philo_main->arg.time_to_die);
	if (get_time_ms() - philo->last_time_eat >= philo->philo_main->arg.time_to_die)
	{
		pthread_mutex_lock(&philo->mtx);
		philo->is_dead = 1;
		pthread_mutex_unlock(&philo->mtx);
		if (!print_status(philo, DIED_STATUS))
			return (0);
		return (1);
	}
	return (0);
}

int	error(t_philo *philo)
{
	pthread_mutex_lock(&philo->philo_main->mtx);
	if (philo->philo_main->error)
	{
		pthread_mutex_unlock(&philo->philo_main->mtx);
		return (1);
	}
	pthread_mutex_unlock(&philo->philo_main->mtx);
	return (0);
}

int	is_finished(t_philo *philo)
{
	int	ret;

	printf("philo %d : is_finished?\n", philo->index);
	if (philo->philo_main->arg.max_eat != -1)
		ret = eat_target_reached(philo) || philo_dead(philo) || error(philo);
	else
		ret = philo_dead(philo) || error(philo);
	printf("philo %d : finished : %d\n", philo->index, ret);
	return (ret);
}

int	print_status(t_philo *philo, char *status)
{
	size_t	time;

	pthread_mutex_lock(&philo->philo_main->mtx);
	time = philo->philo_main->time;
	if (is_finished(philo))
		return (0);
	printf("%zu %d %s", time, philo->index, status);
	return (1);
}

int	eat_routine(t_philo *philo)
{
	printf("philo %d start eat\n", philo->index);
	if (philo->index % 2 == 0)
		pthread_mutex_lock(&philo->right->mtx);
	else
		pthread_mutex_lock(&philo->left->mtx);
	if (!print_status(philo, FORK_STATUS))
		return (0);
	if (philo->index % 2 == 0)
		pthread_mutex_lock(&philo->left->mtx);
	else
		pthread_mutex_lock(&philo->right->mtx);
	if (!print_status(philo, FORK_STATUS))
		return (0);
	pthread_mutex_lock(&philo->mtx);
	philo->nb_eat++;
	pthread_mutex_unlock(&philo->mtx);
	if (!print_status(philo, EAT_STATUS))
		return (0);
	usleep(philo->philo_main->arg.time_to_eat * 1000);
	return (1);
}

int	sleep_routine(t_philo *philo)
{
	if (!print_status(philo, SLEEP_STATUS))
		return (0);
	usleep(philo->philo_main->arg.time_to_sleep * 1000);
	return (1);
}

int	think_routine(t_philo *philo)
{
	(void)philo;
	return (1);
}

int	is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->mtx);
	if (philo->is_dead)
	{
		pthread_mutex_unlock(&philo->mtx);
		return (1);
	}
	pthread_mutex_unlock(&philo->mtx);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->mtx);
	philo->is_ready = 1;
	pthread_mutex_unlock(&philo->mtx);
	printf("philo %d ready\n", philo->index);
	while (1)
	{
		pthread_mutex_lock(&philo->philo_main->mtx);
		if (philo->philo_main->is_running)
		{
			pthread_mutex_unlock(&philo->philo_main->mtx);
			break ;
		}
		pthread_mutex_unlock(&philo->philo_main->mtx);
		// printf("philo %d waiting for monitoring to start\n", philo->index);
	}
	printf("philo %d start\n", philo->index);
	while (!is_finished(philo))
	{
		if (is_dead(philo))
			break ;
		eat_routine(philo);
		sleep_routine(philo);
		think_routine(philo);
	}
	printf("philo %d end\n", philo->index);
	return (NULL);
}

static int	create_threads(t_philo_main *philo_main)
{
	int	i;
	int	nb_philo;

	i = 0;
	nb_philo = philo_main->arg.nb_philo;
	while (i < nb_philo)
	{
		if (pthread_create(&philo_main->philos[i].th, NULL, &routine, \
			&philo_main->philos[i]) != 0)
		{
			pthread_mutex_lock(&philo_main->mtx);
			philo_main->error = 1;
			pthread_mutex_unlock(&philo_main->mtx);
			return (print_error(THREAD_ERROR, 0));
		}
		i++;
	}
	return (1);
}

static int	join_threads(t_philo_main *philo_main)
{
	int	i;

	i = 0;
	while (i < philo_main->arg.nb_philo)
	{
		if (pthread_join(philo_main->philos[i].th, NULL) != 0)
		{
			pthread_mutex_lock(&philo_main->mtx);
			philo_main->error = 1;
			pthread_mutex_unlock(&philo_main->mtx);
			return (print_error(JOIN_ERROR, 0));
		}
		i++;
	}
	if (pthread_join(philo_main->monitoring.th, NULL) != 0)
	{
		pthread_mutex_lock(&philo_main->mtx);
		philo_main->error = 1;
		pthread_mutex_unlock(&philo_main->mtx);
		return (print_error(JOIN_ERROR, 0));
	}
	return (1);
}

int	run(t_philo_main *philo_main)
{
	if (!create_threads(philo_main))
		return (0);
	start_monitoring(philo_main);
	if (!join_threads(philo_main))
		return (0);
	return (1);
}
