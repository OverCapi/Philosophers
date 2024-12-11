/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:31:14 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/11 14:19:36 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_status(t_philo *philo, char *status)
{
	(void)philo;
	(void)status;
	return (1);
}

int	eat_routine(t_philo *philo)
{
	(void)philo;
	return (1);
}

int	sleep_routine(t_philo *philo)
{
	(void)philo;
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
			return (print_error(THREAD_ERROR, 0));
		i++;
	}
	return (1);
}

int	run(t_philo_main *philo_main)
{
	if (!create_threads(philo_main))
		return (0);
	// if (!join_threads(philo_main))
	// 	return (0);
	return (1);
}
