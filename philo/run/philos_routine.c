/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:52:59 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/17 18:04:38 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	*alone_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	set_int_mutex(&philo->mtx, &philo->is_ready, 1);
	if (!wait_start(philo))
		return (NULL);
	pthread_mutex_lock(philo->right_fork);
	if (!print_status(philo, FORK_STATUS))
		return (NULL);
	while (!is_finished(philo))
		continue ;
	return (NULL);
}

void	*even_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	set_int_mutex(&philo->mtx, &philo->is_ready, 1);
	if (!wait_start(philo))
		return (NULL);
	if (!wait_can_eat(philo))
		return (NULL);
	while (!is_finished(philo))
	{
		if (!routine(philo))
			return (NULL);
	}
	return (NULL);
}

void	*philos_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	set_int_mutex(&philo->mtx, &philo->is_ready, 1);
	if (!wait_start(philo))
		return (NULL);
	if (philo->index % 2 == 1)
		ft_usleep(1000);
	while (!is_finished(philo))
	{
		if (!routine(philo))
			return (NULL);
	}
	return (NULL);
}
