/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 14:56:27 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/11 14:20:55 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	destroy_mutex(t_fork *forks, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&forks[i].mtx);
		i++;
	}
	free(forks);
}

static int	init_fork(t_philo_main *philo_main)
{
	int	i;
	int	nb_philo;

	nb_philo = philo_main->arg.nb_philo;
	philo_main->forks = (t_fork *)malloc(nb_philo * sizeof(t_fork));
	if (!philo_main->forks)
		return (print_error(ALLOC_ERROR, 0));
	i = 0;
	while (i < nb_philo)
	{
		if (pthread_mutex_init(&philo_main->forks[i].mtx, NULL) != 0)
			return (destroy_mutex(philo_main->forks, i), 0);
		i++;
	}
	return (1);
}

static int	init_philo(t_philo_main *philo_main, int index)
{
	int	nb_philo;

	nb_philo = philo_main->arg.nb_philo;
	memset(&philo_main->philos[index], 0, sizeof(t_philo));
	philo_main->philos[index].index = index;
	philo_main->philos[index].philo_main = philo_main;
	if (pthread_mutex_init(&philo_main->philos[index].mtx, NULL) != 0)
		return (0);
	philo_main->philos[index].right = &philo_main->forks[index];
	if (index == 0)
		philo_main->philos[index].left = &philo_main->forks[nb_philo - 1];
	else
		philo_main->philos[index].left = &philo_main->forks[index - 1];
	return (1);
}

static int	clean_philo(t_philo_main *philo_main, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&philo_main->philos[i].mtx);
		i++;
	}
	free(philo_main->philos);
	return (0);
}

int	init_philos(t_philo_main *philo_main)
{
	int	i;
	int	nb_philo;

	i = 0;
	nb_philo = philo_main->arg.nb_philo;
	philo_main->philos = (t_philo *)malloc(nb_philo * sizeof(t_philo));
	if (!philo_main->philos)
		return (0);
	while (i < nb_philo)
	{
		if (!init_philo(philo_main, i))
			return (clean_philo(philo_main, i));
		i++;
	}
	return (1);
}

int	init(t_philo_main *philo_main)
{
	if (pthread_mutex_init(&philo_main->mtx, NULL) != 0)
		return (0);
	if (!init_fork(philo_main))
		return (print_error(INIT_ERROR, 0));
	if (!init_philos(philo_main))
		return (destroy_mutex(philo_main->forks, philo_main->arg.nb_philo), 0);
	return (1);
}
