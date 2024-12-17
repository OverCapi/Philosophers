/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:11:12 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/17 18:37:04 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	is_finished(t_philo *philo)
{
	static int	alr_dead = 0;
	int			is_running;
	int			eat_count;

	is_running = get_int_mutex(&philo->prog->mtx, &philo->prog->is_running);
	if (get_int_mutex(&philo->mtx, &philo->is_dead) == 1 && alr_dead != 1)
	{
		alr_dead = 1;
		pthread_mutex_lock(&philo->prog->write_perm);
		printf("%zu %d %s", \
			get_size_t_mutex(&philo->prog->mtx, &philo->prog->time), \
			philo->index, DIED_STATUS);
		pthread_mutex_unlock(&philo->prog->write_perm);
	}
	if (philo->prog->arg.max_eat != -1)
	{
		eat_count = get_int_mutex(&philo->mtx, &philo->eat_count);
		if (eat_count >= philo->prog->arg.max_eat)
			return (1);
	}
	return (!is_running);
}

int	wait_can_eat(t_philo *philo)
{
	while (1)
	{
		if (is_finished(philo))
			return (0);
		if (philo->index % 2 == \
			get_int_mutex(&philo->prog->mtx, &philo->prog->who_can_eat))
			return (1);
	}
	return (1);
}

int	print_status(t_philo *philo, char *status)
{
	size_t		time;

	time = get_size_t_mutex(&philo->prog->mtx, &philo->prog->time);
	if (is_finished(philo))
		return (0);
	pthread_mutex_lock(&philo->prog->write_perm);
	printf("%zu %d %s", time, philo->index, status);
	pthread_mutex_unlock(&philo->prog->write_perm);
	return (1);
}

int	wait_start(t_philo *philo)
{
	while (1)
	{
		if (get_int_mutex(&philo->mtx, &philo->is_dead))
			return ((void)print_status(philo, DIED_STATUS), 0);
		if (get_int_mutex(&philo->prog->mtx, &philo->prog->error))
			return (0);
		if (get_int_mutex(&philo->prog->mtx, &philo->prog->is_running))
			return (1);
	}
	return (0);
}
