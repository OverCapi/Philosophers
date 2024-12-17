/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:11:08 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/17 16:22:04 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static size_t	get_time(int unit)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	if (unit == 0)
		return (time.tv_sec * 1000 + time.tv_usec / 1000);
	else if (unit == 1)
		return (time.tv_sec * 1000000 + time.tv_usec);
	else
		return (0);
}

void	ft_usleep(size_t time_us)
{
	size_t	start_time;
	size_t	elapsed_time;
	size_t	sleep_time;

	start_time = get_time(1);
	elapsed_time = 0;
	sleep_time = time_us / 2;
	while (elapsed_time < time_us)
	{
		usleep(sleep_time);
		elapsed_time = get_time(1) - start_time;
		if (time_us - elapsed_time > 1000)
			sleep_time = (time_us - elapsed_time) / 2;
		else
			sleep_time = 1000;
	}
}

size_t	get_time_since_start(t_prog *prog)
{
	size_t	current_time;

	current_time = get_time(0);
	pthread_mutex_lock(&prog->mtx);
	if (prog->start_time == 0)
		prog->start_time = current_time;
	else
		current_time -= prog->start_time;
	pthread_mutex_unlock(&prog->mtx);
	return (current_time);
}

void	update_time(t_prog *prog)
{
	size_t	current_time;

	current_time = get_time(0);
	pthread_mutex_lock(&prog->mtx);
	if (prog->start_time == 0)
		prog->start_time = current_time;
	else
		current_time -= prog->start_time;
	prog->time = current_time;
	pthread_mutex_unlock(&prog->mtx);
}
