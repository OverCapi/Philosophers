/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 13:01:09 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/09 14:17:58 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_arg(t_arg *arg, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return ((void)printf("Error : %s", ERROR_ARG), 0);
	arg->nb_philo = ft_atoi_safe(argv[1]);
	arg->time_to_die = ft_atoi_safe(argv[2]);
	arg->time_to_eat = ft_atoi_safe(argv[3]);
	arg->time_to_sleep = ft_atoi_safe(argv[4]);
	if (argc == 6)
		arg->nbt_philo_each_must_eat = ft_atoi_safe(argv[5]);
	return (1);
}

int	init_philo_child(t_philo_main *philo_main)
{
	int	n;

	n = 0;
	philo_main->philo = (t_philo_child *)malloc(philo_main->arg.nb_philo * sizeof(t_philo_child));
	if (!philo_main->philo)
		return ((void)printf("Error : %s", ERROR_ALLOC), 0);
	while (n < philo_main->arg.nb_philo)
	{
		philo_main->philo[n].index = n;
		philo_main->philo[n].philo_main = philo_main;
		if (pthread_create(&philo_main->philo[n].th, NULL, &routine, &philo_main->philo[n]) != 0)
			return (free(philo_main->philo), 0);
		n++;
	}
	return (1);
}

int	destroy_mutex(t_philo_main *philo_main, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pthread_mutex_destroy(&philo_main->svar.fork[n].mutex) != 0)
			printf("mutex destroy fail\n");
		i++;
	}
	free(philo_main->svar.fork);
	return (0);
}

int	init_mutex(t_philo_main *philo_main)
{
	int	n;

	philo_main->svar.fork = (t_fork *)malloc(philo_main->arg.nb_philo * sizeof(t_fork));
	if (!philo_main->svar.fork)
		return ((void)printf("Error : %s", ERROR_ALLOC), 0);
	n = 0;
	while (n < philo_main->arg.nb_philo)
	{
		if (pthread_mutex_init(&philo_main->svar.fork[n].mutex, NULL) != 0)
			return (destroy_mutex(philo_main, n));
		n++;
	}
	return (1);
}

int	init_philo_main(t_philo_main *philo_main)
{
	if (!init_mutex(philo_main))
		return ((void)printf("Error : %s", ERROR_INIT), 0);
	if (!init_philo_child(philo_main))
		return ((void)printf("Error : %s", ERROR_INIT), 0);
	return (1);
}

int	join_philo(t_philo_main *philo_main)
{
	int	n;

	n = 0;
	while (n < philo_main->arg.nb_philo)
	{
		if (pthread_join(philo_main->philo[n].th, NULL) != 0)
			return (free(philo_main->philo), 0);
		n++;
	}
	return (1);
}
