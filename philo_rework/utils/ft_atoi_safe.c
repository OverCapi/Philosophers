/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_safe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:29:21 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/16 14:29:35 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static int	ft_iswhite_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

static int	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

static int	check_overflow(long long int nb, int sign)
{
	if (sign == 1 && nb > 2147483647)
		return (-1);
	else if (sign == -1 && (nb < -2147483648 || nb >= 2147483647))
		return (-1);
	return (0);
}

int	ft_atoi_safe(const char *nptr)
{
	int	nb;
	int	sign;
	int	i;

	i = 0;
	nb = 0;
	sign = 1;
	while (ft_iswhite_space(nptr[i]) == 1 && nptr[i] != '\0')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(nptr[i]) == 1)
	{
		if (check_overflow((long long int)nb * 10 + nptr[i] - '0', sign) == -1)
			return (0);
		nb = nb * 10 + nptr[i] - '0';
		i++;
	}
	return (nb * sign);
}