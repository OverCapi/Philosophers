/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 09:22:18 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/10 11:02:12 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_arg(t_arg *arg, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (print_error(ARG_ERROR, 1));
	arg->nb_philo = ft_atoi_safe(argv[1]);
	arg->time_to_die = ft_atoi_safe(argv[2]);
	arg->time_to_eat = ft_atoi_safe(argv[3]);
	arg->time_to_sleep = ft_atoi_safe(argv[4]);
	if (argc == 6)
		arg->max_eat = ft_atoi_safe(argv[5]);
	else
		arg->max_eat = -1;
	if (arg->nb_philo <= 0 || arg->time_to_die <= 0 || arg->time_to_eat <= 0 \
		|| arg->time_to_sleep <= 0 || (arg->max_eat < -1 || arg->max_eat == 0))
		return (print_error(ARG_VALUE_ERROR, 0));
	return (0);
}

int	init_fork(t_philo_main *philo_main)
{
	int	i;
	int	nb_philo;

	i = 0;
	nb_philo = philo_main->arg.nb_philo;
	philo_main->forks = (t_fork *)malloc(nb_philo * sizeof(t_fork));
	if (!philo_main->forks)
		return (print_error(ALLOC_ERROR, 0));
	while (i < nb_philo)
	{
		if (pthread_mutex_init(&philo_main->forks[i], NULL) != 0)
		{
			destroy_mutex(philo_main->forks, i);
			return (print_error(INIT_ERROR, 0));
		}
		i++;
	}
}

int	init(t_philo_main *philo_main)
{
	if (!init_fork(philo_main))
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	t_philo_main	philo_main;

	memset(&philo_main, 0, sizeof(t_philo_main));
	if (!parse_arg(&philo_main.arg, argc, argv))
		return (1);
	if (!init(&philo_main))
		return (1);
	return (0);
}
