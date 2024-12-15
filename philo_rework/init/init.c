#include "../philo.h"

int	init(t_prog *prog)
{
	if (!init_mutex(prog))
		return (print_error(INIT_ERROR, 0));
	if (!init_philos(prog))
	{
		clean_mutex(prog);
		return (print_error(INIT_ERROR, 0));
	}
}
