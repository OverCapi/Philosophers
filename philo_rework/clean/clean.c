#include "../philo.h"

void	clean(t_prog *prog)
{
	clean_mutex(prog);
	clean_philos(&prog->philos, prog->arg.nb_philo);
}
