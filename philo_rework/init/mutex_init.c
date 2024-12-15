#include "../philo.h"

static int	init_forks(t_prog *prog)
{
	int	i;

	i = -1;
	while (++i < prog->arg.nb_philo)
	{
		if (pthread_mutex_init(&prog->forks[i], NULL) != 0)
		{
			pthread_mutex_destroy(&prog->mtx);
			pthread_mutex_destroy(&prog->write_perm);
			return (destroy_mutex(&prog->forks, i), 0);
		}
	}
}

int	init_mutex(t_prog *prog)
{
	if (pthread_mutex_init(&prog->mtx, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&prog->write_perm, NULL) != 0)
		return ((void)pthread_mutex_destroy(&prog->mtx), 0);
	if (!init_forks(prog))
	{
		pthread_mutex_destroy(&prog->mtx);
		pthread_mutex_destroy(&prog->write_perm);
		return (0);
	}
}
