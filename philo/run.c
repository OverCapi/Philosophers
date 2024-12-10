/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:31:14 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/10 16:28:06 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	eat(t_philo *philo)
{
	int	err;

	if (philo->index % 2 == 0)
		pthread_mutex_lock(&philo->left->mtx);
	else
		pthread_mutex_lock(&philo->right->mtx);
	if (philo->index % 2 == 0)
		pthread_mutex_lock(&philo->right->mtx);
	else
		pthread_mutex_lock(&philo->left->mtx);
	if (!get_int_mutex(&philo->philo_main->mtx, &philo->philo_main->is_running, &err))
		return (0);
	pthread_mutex_lock(&philo->philo_main->mtx);
	print_status(philo, "eating");
	pthread_mutex_unlock(&philo->philo_main->mtx);
	usleep(philo->philo_main->arg.time_to_eat * 1000);
	pthread_mutex_unlock(&philo->left->mtx);
	pthread_mutex_unlock(&philo->right->mtx);
	return (1);
}

int	sleep(t_philo *philo)
{
	
	return (1);
}

int	think(t_philo *philo)
{
	
	return (1);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	int		err;

	philo = (t_philo *)arg;
	if (!set_int_mutex(&philo->mtx, &philo->is_ready, 1))
		return ((void *)print_error("LOCK OR UNLOCK MUTEX ERROR", 0));
	while (1)
	{
		if (get_int_mutex(&philo->philo_main->mtx, &philo->philo_main->is_running, &err))
			break ;
		if (err == 1)
			break ;
	}
	if (err == 1)
		return ((void *)print_error("LOCK OR UNLOCK MUTEX ERROR", 0));
	if (!eat(philo))
		return (NULL);
	if (!sleep(philo))
		return (NULL);
	if (!think(philo))
		return (NULL);
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
		if (pthread_create(&philo_main->philos[i].th, NULL, &routine, &philo_main->philos[i]) != 0)
			return (print_error(THREAD_ERROR, 0));
		i++;
	}
	return (1);
}

int	run(t_philo_main *philo_main)
{
	if (!create_threads(philo_main))
		return (0);
	return (1);
}
