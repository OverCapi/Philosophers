/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_even.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 15:34:37 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/14 16:03:52 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	eat_routine_even(t_philo *philo)
{
	if (philo->index % 2 == 0)
		pthread_mutex_lock(&philo->left->mtx);
	else
		pthread_mutex_lock(&philo->right->mtx);
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
	philo->nb_eat++;
	pthread_mutex_unlock(&philo->mtx);
	pthread_mutex_unlock(&philo->right->mtx);
	pthread_mutex_unlock(&philo->left->mtx);
	return (1);
}

int	sleep_routine_even(t_philo *philo)
{
	if (!print_status(philo, SLEEP_STATUS))
		return (0);
	ft_usleep(philo->philo_main->arg.time_to_sleep * 1000);
	return (1);
}

int	think_routine_even(t_philo *philo)
{
	print_status(philo, THINK_STATUS);
	ft_usleep(30);
	return (1);
}

void	*even_routine(void *arg)
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
	if (!is_finished(philo) && philo->index % 2 == 0)
	{
		eat_routine_even(philo);
		if (is_finished(philo))
			return (NULL);
		sleep_routine_even(philo);
		if (is_finished(philo))
			return (NULL);
		think_routine_even(philo);
	}
	while (!is_finished(philo))
	{
		eat_routine_even(philo);
		if (is_finished(philo))
			break ;
		sleep_routine_even(philo);
		if (is_finished(philo))
			break ;
		think_routine_even(philo);
	}
	return (NULL);
}

int	philo_even(t_philo_main *philo_main)
{
	long int	nb_philo;
	long int	i;

	i = 0;
	nb_philo = philo_main->arg.nb_philo;
	while (i < nb_philo)
	{
		if (pthread_create(&philo_main->philos[i].th, NULL, &even_routine, \
			&philo_main->philos[i]) != 0)
		{
			pthread_mutex_lock(&philo_main->mtx);
			philo_main->error = 1;
			pthread_mutex_unlock(&philo_main->mtx);
			return (print_error(THREAD_ERROR, 0));
		}
		i++;
	}
	start_monitoring(philo_main);
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
