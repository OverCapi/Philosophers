/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llemmel <llemmel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:10:47 by llemmel           #+#    #+#             */
/*   Updated: 2024/12/17 18:42:25 by llemmel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_prog	prog;

	memset(&prog, 0, sizeof(t_prog));
	if (!parse_arg(&prog.arg, argc, argv))
		return (1);
	if (!init(&prog))
		return (1);
	run(&prog);
	clean(&prog);
	return (0);
}
