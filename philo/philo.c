/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:11:20 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/06 14:17:02 by llemmel          ###   ########.fr       */
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

int	main(int argc, char **argv)
{
	t_arg	arg;

	memset(&arg, 0, sizeof(arg));
	if (init_arg(&arg, argc, argv) != 0)
		return (1);
	return (0);
}
