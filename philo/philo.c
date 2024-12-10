/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 09:22:18 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/10 15:33:00 by llemmel          ###   ########.fr       */
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

int	main(int argc, char **argv)
{
	t_philo_main	philo_main;

	memset(&philo_main, 0, sizeof(t_philo_main));
	if (!parse_arg(&philo_main.arg, argc, argv))
		return (1);
	if (!init(&philo_main))
		return (1);
	if (!run(&philo_main))
		return (1);
	clean(&philo_main);
	return (0);
}
