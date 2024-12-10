/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 14:56:27 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/10 16:18:32 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	destroy_mutex(t_fork **forks, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&(*forks)[i]);
		i++;
	}
}

static int	init_fork(t_fork **forks, int nb_philo)
{
	int	i;

	i = 0;
	*forks = (t_fork *)malloc(nb_philo * sizeof(t_fork));
	if (!*forks)
		return (print_error(ALLOC_ERROR, 0));
	while (i < nb_philo)
	{
		if (pthread_mutex_init(&(*forks)[i], NULL) != 0)
		{
			destroy_mutex(&forks, i);
			return (free(*forks), 0);
		}
		i++;
	}
	return (1);
}

static int	init_philo(t_philo_main *philo_main)
{
	int	i;
	int	nb_philo;

	i = -1;
	nb_philo = philo_main->arg.nb_philo;
	philo_main->philos = (t_philo *)malloc(nb_philo * sizeof(t_philo));
	if (!philo_main->philos)
		return (print_error(ALLOC_ERROR, 0));
	while (++i < nb_philo)
	{
		memset(&philo_main->philos[i], 0, sizeof(t_philo));
		if (pthread_mutex_init(&philo_main->philos[i].mtx, NULL) != 0)
		{
			while (i >= 0)
				pthread_mutex_destroy(&philo_main->philos[i--].mtx);
			return (free(philo_main->philos), 0);
		}
		philo_main->philos[i].index = i;
		philo_main->philos[i].philo_main = &philo_main;
		philo_main->philos[i].right = &philo_main->forks[i];
		if (i != 0)
			philo_main->philos[i].left = &philo_main->forks[i - 1];
		else
			philo_main->philos[i].left = &philo_main->forks[nb_philo - 1];
	}
	return (1);
}

int	init(t_philo_main *philo_main)
{
	if (pthread_mutex_init(&philo_main->mtx, NULL) != 0)
		return (print_error(INIT_ERROR, 0));
	if (!init_fork(&philo_main->forks, philo_main->arg.nb_philo))
	{
		pthread_mutex_destroy(&philo_main->mtx);
		return (print_error(INIT_ERROR, 0));
	}
	if (!init_philo(philo_main))
	{
		pthread_mutex_destroy(&philo_main->mtx);
		destroy_mutex(&philo_main->forks, philo_main->arg.nb_philo);
		free(philo_main->forks);
		return (print_error(INIT_ERROR, 0));
	}
	return (1);
}
