/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:31:14 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/12 14:29:23 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int	get_time_ms(t_philo_main *philo_main)
{
	struct timeval	time;
	size_t			current_time;

	pthread_mutex_lock(&philo_main->mtx);
	gettimeofday(&time, NULL);
	current_time = time.tv_sec * 1000 + time.tv_usec / 1000;
	if (philo_main->start_time == 0)
		philo_main->start_time = current_time;
	pthread_mutex_unlock(&philo_main->mtx);
	return (current_time - philo_main->start_time);
}

int	eat_target_reached(t_philo *philo)
{
	return (philo->nb_eat >= philo->philo_main->arg.max_eat);
}

int	philo_dead(t_philo *philo)
{
	// printf("philo %d : philo_dead : %ld, (%ld)\n", philo->index,get_time_ms(philo->philo_main) - philo->last_time_eat, philo->philo_main->arg.time_to_die);
	pthread_mutex_lock(&philo->mtx);
	if (get_time_ms(philo->philo_main) - philo->last_time_eat >= philo->philo_main->arg.time_to_die)
	{
		philo->is_dead = 1;
		pthread_mutex_unlock(&philo->mtx);
		if (!print_status(philo, DIED_STATUS))
			return (0);
		return (1);
	}
	pthread_mutex_unlock(&philo->mtx);
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

	// printf("philo %d check if sim is finished\n", philo->index);
	pthread_mutex_lock(&philo->philo_main->mtx);
	ret = philo->philo_main->is_running;
	pthread_mutex_unlock(&philo->philo_main->mtx);
	pthread_mutex_lock(&philo->mtx);
	if (philo->is_dead)
	{
		pthread_mutex_unlock(&philo->mtx);
		pthread_mutex_lock(&philo->philo_main->can_write);
		// printf("philo %d end check if sim is finished\n", philo->index);
		printf("%zums %d %s", get_time_ms(philo->philo_main), philo->index, DIED_STATUS);
		pthread_mutex_unlock(&philo->philo_main->can_write);
		return (1);
	}
	pthread_mutex_unlock(&philo->mtx);
	// printf("philo %d end check if sim is finished\n", philo->index);
	// printf("%d running status : %d, ret value : %d\n", philo->index, ret, !ret);
	return (!ret);
}

int	print_status(t_philo *philo, char *status)
{
	size_t	time;

	time = philo->philo_main->time;
	if (is_finished(philo))
		return (0);
	pthread_mutex_lock(&philo->philo_main->can_write);
	printf("%zums %d %s", time, philo->index, status);
	pthread_mutex_unlock(&philo->philo_main->can_write);
	return (1);
}

int	eat_routine(t_philo *philo)
{
	if (philo->index % 2 == 0)
		pthread_mutex_lock(&philo->left->mtx);
	else
		pthread_mutex_lock(&philo->right->mtx);
	if (!print_status(philo, FORK_STATUS))
		return (0);
	if (philo->index % 2 == 0)
		pthread_mutex_lock(&philo->right->mtx);
	else
		pthread_mutex_lock(&philo->left->mtx);
	if (!print_status(philo, FORK_STATUS))
		return (0);
	if (!print_status(philo, EAT_STATUS))
		return (0);
	pthread_mutex_lock(&philo->mtx);
	philo->last_time_eat = get_time_ms(philo->philo_main);
	// printf("philo %d : last_time_eat : %d\n", philo->index, philo->last_time_eat);
	pthread_mutex_unlock(&philo->mtx);
	usleep(philo->philo_main->arg.time_to_eat * 1000);
	pthread_mutex_lock(&philo->mtx);
	philo->nb_eat++;
	pthread_mutex_unlock(&philo->mtx);
	pthread_mutex_unlock(&philo->right->mtx);
	pthread_mutex_unlock(&philo->left->mtx);
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

void	*routine(void *arg)
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
	while (!is_finished(philo))
	{
		if (is_finished(philo))
			break ;
		eat_routine(philo);
		if (is_finished(philo))
			break ;
		sleep_routine(philo);
		if (is_finished(philo))
			break ;
		think_routine(philo);
	}
	printf("philo %d : finished\n", philo->index);
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
	printf("dinner finished\n");
	return (1);
}
