/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:11:20 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/09 13:41:39 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_philo_main	philo_main;

	memset(&philo_main, 0, sizeof(philo_main));
	if (!init_arg(&philo_main.arg, argc, argv))
		return (1);
	if (!init_philo_main(&philo_main))
		return (1);
	run(&philo_main);
	if (!join_philo(&philo_main))
		return (1);
	destroy_mutex(&philo_main, philo_main.arg.nb_philo);
	free(philo_main.philo);
	return (0);
}
