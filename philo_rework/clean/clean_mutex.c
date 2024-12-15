#include "../philo.h"

void	destroy_mutex(pthread_mutex_t **forks, int size)
{
	int	i;

	i = -1;
	while (++i < size)
		pthread_mutex_destroy(&(*forks)[i]);
	free(*forks);
	*forks = NULL;
}

void	clean_mutex(t_prog *prog)
{
	pthread_mutex_destroy(&prog->write_perm);
	pthread_mutex_destroy(&prog->mtx);
	destroy_mutex(&prog->forks, prog->arg.nb_philo);
}
