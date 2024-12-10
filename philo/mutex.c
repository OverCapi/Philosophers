/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:57:03 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/10 16:03:50 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	set_int_mutex(pthread_mutex_t *mtx, int *ptr, int value)
{
	if (pthread_mutex_lock(mtx) != 0)
		return (-1);
	*ptr = value;
	if (pthread_mutex_unlock(mtx) != 0)
		return (-1);
}

int	get_int_mutex(pthread_mutex_t *mtx, int *ptr, int *err)
{
	int	ret;

	ret = 0;
	*err = 0;
	if (pthread_mutex_lock(mtx) != 0)
	{
		*err = 1;
		return (ret);
	}
	ret = *ptr;
	if (pthread_mutex_unlock(mtx) != 0)
	{
		*err = 1;
		return (ret);
	}
	return (ret);
}
