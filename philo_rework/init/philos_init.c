#include "../philo.h"

static int	init_philo(t_prog *prog, int index)
{
	if (pthread_mutex_init(&prog->philos[index].mtx, NULL) != 0)
		return (0);
	prog->philos[index].index = index;
	prog->philos[index].right_fork = &prog->forks[index];
	if (index != 0)
		prog->philos[index].left_fork = &prog->forks[index - 1];
	else
		prog->philos[index].left_fork = &prog->forks[prog->arg.nb_philo - 1];
	prog->philos[index].prog = prog;
	return (1);
}

int	init_philos(t_prog *prog)
{
	int	i;

	i = -1;
	prog->philos = (t_philo *)malloc(sizeof(t_philo) * prog->arg.nb_philo);
	if (!prog->philos)
		return (0);
	while (++i < prog->arg.nb_philo)
	{
		memset(&prog->philos[i], 0, sizeof(t_philo));
		if (!init_philo(prog, i))
			return (clean_philos(&prog->philos, i), 0);
	}
	return (1);
}