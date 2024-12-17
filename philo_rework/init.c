/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:10:32 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/17 14:10:34 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_forks(t_prog *prog)
{
	int	i;

	i = -1;
	prog->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * \
		prog->arg.nb_philo);
	if (!prog->forks)
		return (0);
	while (++i < prog->arg.nb_philo)
	{
		if (pthread_mutex_init(&prog->forks[i], NULL) != 0)
		{
			pthread_mutex_destroy(&prog->mtx);
			pthread_mutex_destroy(&prog->write_perm);
			return (destroy_mutex(prog, i), 0);
		}
	}
	return (1);
}

static int	init_mutex(t_prog *prog)
{
	if (pthread_mutex_init(&prog->mtx, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&prog->write_perm, NULL) != 0)
		return ((void)pthread_mutex_destroy(&prog->mtx), 0);
	if (!init_forks(prog))
	{
		pthread_mutex_destroy(&prog->mtx);
		pthread_mutex_destroy(&prog->write_perm);
		return (0);
	}
	return (1);
}

static int	init_philo(t_prog *prog, int index)
{
	if (pthread_mutex_init(&prog->philos[index].mtx, NULL) != 0)
		return (0);
	prog->philos[index].index = index;
	prog->philos[index].right_fork = &prog->forks[index];
	if (index != 0)
		prog->philos[index].left_fork = &prog->forks[index - 1];
	else
		prog->philos[index].left_fork = &prog->forks[prog->arg.nb_philo - 1];
	prog->philos[index].prog = prog;
	return (1);
}

static int	init_philos(t_prog *prog)
{
	int	i;

	i = -1;
	prog->philos = (t_philo *)malloc(sizeof(t_philo) * prog->arg.nb_philo);
	if (!prog->philos)
		return (0);
	while (++i < prog->arg.nb_philo)
	{
		memset(&prog->philos[i], 0, sizeof(t_philo));
		if (!init_philo(prog, i))
			return (clean_philos(prog, i), 0);
	}
	return (1);
}

int	init(t_prog *prog)
{
	if (!init_mutex(prog))
		return (print_error(INIT_ERROR, 0));
	if (!init_philos(prog))
	{
		clean_mutex(prog);
		return (print_error(INIT_ERROR, 0));
	}
	return (1);
}
