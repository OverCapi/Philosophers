/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:02:19 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/18 17:07:29 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static int	wait_all_threads_ready(t_prog *prog)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < prog->arg.nb_philo)
		{
			if (get_int_mutex(&prog->mtx, &prog->error))
				return (0);
			if (!get_int_mutex(&prog->philos[i].mtx, &prog->philos[i].is_ready))
				break ;
		}
		if (i == prog->arg.nb_philo)
			return (1);
	}
	return (1);
}

static void	check_who_can_eat(t_prog *prog)
{
	static int	nb_call = 1;
	int			who_can_eat;
	int			i;

	who_can_eat = get_int_mutex(&prog->mtx, &prog->who_can_eat);
	i = who_can_eat;
	while (i < prog->arg.nb_philo)
	{
		if (get_int_mutex(&prog->philos[i].mtx, &prog->philos[i].eat_count) \
			< nb_call)
			return ;
		i += 2;
	}
	nb_call += 1;
	set_int_mutex(&prog->mtx, &prog->who_can_eat, (who_can_eat + 1) % 2);
}

static int	philos_dead(t_prog *prog)
{
	int		i;
	size_t	last_time_eat;
	size_t	current_time;
	size_t	time_to_die;

	i = -1;
	time_to_die = (size_t)(prog->arg.time_to_die + 5);
	while (++i < prog->arg.nb_philo)
	{
		last_time_eat = get_size_t_mutex(&prog->philos[i].mtx, \
			&prog->philos[i].last_time_eat);
		current_time = get_size_t_mutex(&prog->mtx, &prog->time);
		if (current_time - last_time_eat >= time_to_die)
		{
			set_int_mutex(&prog->philos[i].mtx, &prog->philos[i].is_dead, 1);
			set_int_mutex(&prog->mtx, &prog->is_running, 0);
			return (1);
		}
	}
	return (0);
}

static int	check_philos_full(t_prog *prog)
{
	int	i;
	int	eat_count;

	i = -1;
	while (++i < prog->arg.nb_philo)
	{
		eat_count = get_int_mutex(&prog->philos[i].mtx, \
			&prog->philos[i].eat_count);
		if (eat_count < prog->arg.max_eat)
			return (0);
	}
	set_int_mutex(&prog->mtx, &prog->is_running, 0);
	return (1);
}

void	*monitoring_routine(void *arg)
{
	t_prog	*prog;

	prog = (t_prog *)arg;
	if (!wait_all_threads_ready(prog))
		return (NULL);
	update_time(prog);
	set_int_mutex(&prog->mtx, &prog->is_running, 1);
	while (1)
	{
		check_who_can_eat(prog);
		update_time(prog);
		if (philos_dead(prog))
			return (NULL);
		if (prog->arg.max_eat != -1 && check_philos_full(prog))
			return (NULL);
	}
	return (NULL);
}
