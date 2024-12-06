/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:11:20 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/06 15:24:27 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_arg(t_arg *arg, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return ((void)printf("Error : %s", ERROR_ARG), -1);
	arg->nb_philo = ft_atoi_safe(argv[1]);
	arg->time_to_die = ft_atoi_safe(argv[2]);
	arg->time_to_eat = ft_atoi_safe(argv[3]);
	arg->time_to_sleep = ft_atoi_safe(argv[4]);
	if (argc == 6)
		arg->nbt_philo_each_must_eat = ft_atoi_safe(argv[5]);
	return (0);
}

void	free_philo_main(t_philo_main *philo_main)
{
	free(philo_main->philo);
}

void	*thread_routine(void *thread_id)
{
	printf("thread id : %ld\n", (long)thread_id);
	return (NULL);
}

int	init_philo_main(t_philo_main *philo_main)
{
	long	n;

	n = 0;
	philo_main->philo = (pthread_t *)malloc(sizeof(pthread_t) * philo_main->arg.nb_philo);
	if (!philo_main->philo)
		return ((void)printf("Error : %s", ERROR_ALLOC), -1);
	while (n < philo_main->arg.nb_philo)
	{
		if (pthread_create(&philo_main->philo[n], NULL, &thread_routine, (void *)n) != 0)
			return (free_philo_main(philo_main), -1);
		n++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo_main	philo_main;
	int				n;

	memset(&philo_main, 0, sizeof(philo_main));
	if (init_arg(&philo_main.arg, argc, argv) != 0)
		return (1);
	if (init_philo_main(&philo_main) != 0)
		return (1);
	n = 0;
	while (n < philo_main.arg.nb_philo)
	{
		pthread_join(philo_main.philo[n], NULL);
		n++;
	}
	free_philo_main(&philo_main);
	return (0);
}
