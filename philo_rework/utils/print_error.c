/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:24:19 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/16 14:27:47 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static size_t	ft_strlen(char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

int	print_error(char *msg, int status)
{
	write(2, msg, ft_strlen(msg));
	return (status);
}
