#include "philo.h"

int	parse_arg(t_arg *arg, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (print_error(ARG_ERROR, 0));
	arg->nb_philo = ft_atoi_safe(argv[1]);
	arg->time_to_die = ft_atoi_safe(argv[2]);
	arg->time_to_eat = ft_atoi_safe(argv[3]);
	arg->time_to_sleep = ft_atoi_safe(argv[4]);
	if (argc == 6)
		arg->max_eat = ft_atoi_safe(argv[5]);
	else
		arg->max_eat = -1;
	if (arg->nb_philo <= 0 || arg->time_to_die <= 0 || arg->time_to_eat <= 0 \
		|| arg->time_to_sleep <= 0 || (arg->max_eat < -1 || arg->max_eat == 0))
		return (print_error(ARG_VALUE_ERROR, 0));
	return (1);
}
