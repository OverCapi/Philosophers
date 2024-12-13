/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:31:14 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/13 17:05:07 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// 0 : ms, 1 : us
long int	get_time(int unit)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	if (unit == 0)
		return (time.tv_sec * 1000 + time.tv_usec / 1000);
	else if (unit == 1)
		return (time.tv_sec * 1000000 + time.tv_usec);
	else
		return (0);
}

void	ft_usleep(long int time_us)
{
	long int		start_time;
	long int		remaining;

	if (time_us <= 0)
		return ;
	start_time = get_time(1);
	remaining = time_us;
	while (remaining >= 0)
	{
		remaining = time_us - (get_time(1) - start_time);
		if (remaining <= 1000)
		{
			while (remaining >= 0)
				remaining = time_us - (get_time(1) - start_time);
		}
		else
			usleep(remaining / 2);
	}
}

// get time elapsed since the start of the sim
long int	get_time_ms(t_philo_main *philo_main)
{
	long int	current_time;

	current_time = get_time(0);
	pthread_mutex_lock(&philo_main->mtx);
	if (philo_main->start_time == 0)
		philo_main->start_time = current_time;
	else
		current_time -= philo_main->start_time;
	pthread_mutex_unlock(&philo_main->mtx);
	return (current_time);
}

int	eat_target_reached(t_philo *philo)
{
	return (philo->nb_eat >= philo->philo_main->arg.max_eat);
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
	static int	once = 0;
	int			ret;

	pthread_mutex_lock(&philo->philo_main->mtx);
	ret = philo->philo_main->is_running;
	pthread_mutex_unlock(&philo->philo_main->mtx);
	pthread_mutex_lock(&philo->mtx);
	if (philo->is_dead && !once)
	{
		pthread_mutex_unlock(&philo->mtx);
		pthread_mutex_lock(&philo->philo_main->can_write);
		pthread_mutex_lock(&philo->philo_main->mtx);
		once = 1;
		printf("%zu %d %s", philo->philo_main->time, philo->index, DIED_STATUS);
		pthread_mutex_unlock(&philo->philo_main->can_write);
		pthread_mutex_unlock(&philo->philo_main->mtx);
		return (1);
	}
	pthread_mutex_unlock(&philo->mtx);
	return (!ret);
}

int	print_status(t_philo *philo, char *status)
{
	size_t	time;

	pthread_mutex_lock(&philo->philo_main->mtx);
	time = philo->philo_main->time;
	pthread_mutex_unlock(&philo->philo_main->mtx);
	if (is_finished(philo))
		return (0);
	pthread_mutex_lock(&philo->philo_main->can_write);
	printf("%zu %d %s", time, philo->index, status);
	pthread_mutex_unlock(&philo->philo_main->can_write);
	return (1);
}

int	eat_routine(t_philo *philo)
{
	if (philo->index % 2 == 0)
		pthread_mutex_lock(&philo->left->mtx);
	else
		pthread_mutex_lock(&philo->right->mtx);
	if (is_finished(philo))
			return (0);
	if (!print_status(philo, FORK_STATUS))
	{
		pthread_mutex_unlock(&philo->left->mtx);
		pthread_mutex_unlock(&philo->right->mtx);
		return (0);
	}
	if (philo->index % 2 == 0)
		pthread_mutex_lock(&philo->right->mtx);
	else
		pthread_mutex_lock(&philo->left->mtx);
	if (is_finished(philo))
		return (0);
	if (!print_status(philo, FORK_STATUS))
	{
		pthread_mutex_unlock(&philo->left->mtx);
		pthread_mutex_unlock(&philo->right->mtx);
		return (0);
	}
	if (!print_status(philo, EAT_STATUS))
	{
		pthread_mutex_unlock(&philo->left->mtx);
		pthread_mutex_unlock(&philo->right->mtx);
		return (0);
	}
	ft_usleep(philo->philo_main->arg.time_to_eat * 1000);
	pthread_mutex_lock(&philo->mtx);
	philo->last_time_eat = get_time_ms(philo->philo_main);
	pthread_mutex_unlock(&philo->mtx);
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
	ft_usleep(philo->philo_main->arg.time_to_sleep * 1000);
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
