#include "../philo.h"

void	set_int_mutex(pthread_mutex_t *mtx, int *ptr, int value)
{
	pthread_mutex_lock(mtx);
	*ptr = value;
	pthread_mutex_unlock(mtx);
}

int	get_int_mutex(pthread_mutex_t *mtx, int *ptr)
{
	int	ret;

	pthread_mutex_lock(mtx);
	ret = *ptr;
	pthread_mutex_unlock(mtx);
	return (ret);
}

void	set_size_t_mutex(pthread_mutex_t *mtx, size_t *ptr, size_t value)
{
	pthread_mutex_lock(mtx);
	*ptr = value;
	pthread_mutex_unlock(mtx);
}

size_t	get_size_t_mutex(pthread_mutex_t *mtx, size_t *ptr)
{
	size_t	ret;

	pthread_mutex_lock(mtx);
	ret = *ptr;
	pthread_mutex_unlock(mtx);
	return (ret);
}
