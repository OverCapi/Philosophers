/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 13:03:04 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/09 13:39:52 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *arg)
{
	t_philo_child	*philo;

	philo = (t_philo_child *)arg;
	printf("index of thread : %d\n", philo->index);
	return (NULL);
}

void	run(t_philo_main *philo_main)
{
	(void)philo_main;
	// while (1)
	// 	continue ;
}
