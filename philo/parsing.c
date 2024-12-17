/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:10:54 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/17 14:43:04 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_string(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ((str[i] >= 9 && str[i] <= 13) || str[i] == 32))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			return (0);
		i++;
	}
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
		i++;
	if (str[i])
		return (0);
	return (1);
}

static int	extract_number(int *arg_ptr, char *arg)
{
	if (!check_string(arg))
		return (print_error(WRONG_PARAM_ERROR, 0));
	*arg_ptr = ft_atoi_safe(arg);
	if (*arg_ptr <= 0 || *arg_ptr >= 2147483)
		return (print_error(OVERFLOW_ERROR, 0));
	return (1);
}

int	parse_arg(t_arg *arg, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (print_error(ARG_ERROR, 0));
	if (!extract_number(&arg->nb_philo, argv[1]))
		return (0);
	if (!extract_number(&arg->time_to_die, argv[2]))
		return (0);
	if (!extract_number(&arg->time_to_eat, argv[3]))
		return (0);
	if (!extract_number(&arg->time_to_sleep, argv[4]))
		return (0);
	if (argc == 6)
	{
		if (!extract_number(&arg->max_eat, argv[5]))
			return (0);
	}
	else
		arg->max_eat = -1;
	return (1);
}
