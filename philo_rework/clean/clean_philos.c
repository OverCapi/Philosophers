#include "../philo.h"

void	clean_philos(t_philo **philos, int size)
{
	int	i;

	i = -1;
	while (++i < size)
		pthread_mutex_destroy(&(*philos)[i].mtx);
	free(*philos);
	*philos = NULL;
}
