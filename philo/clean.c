/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:11:01 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/17 14:11:02 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mutex(t_prog *prog, int size)
{
	int	i;

	i = -1;
	while (++i < size)
		pthread_mutex_destroy(&prog->forks[i]);
	free(prog->forks);
	prog->forks = NULL;
}

void	clean_mutex(t_prog *prog)
{
	pthread_mutex_destroy(&prog->write_perm);
	pthread_mutex_destroy(&prog->mtx);
	destroy_mutex(prog, prog->arg.nb_philo);
}

void	clean_philos(t_prog *prog, int size)
{
	int	i;

	i = -1;
	while (++i < size)
		pthread_mutex_destroy(&prog->philos[i].mtx);
	free(prog->philos);
	prog->philos = NULL;
}

void	clean(t_prog *prog)
{
	clean_mutex(prog);
	clean_philos(prog, prog->arg.nb_philo);
}
