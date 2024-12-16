#include "../philo.h"

void	*alone_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	set_int_mutex(&philo->mtx, &philo->is_ready, 1);
	if (!wait_start(philo))
		return (NULL);
	if (!print_status(philo, FORK_STATUS))
		return (NULL);
	while (!is_finished(philo))
		continue ;
	return (NULL);
}

void	*even_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	set_int_mutex(&philo->mtx, &philo->is_ready, 1);
	if (!wait_start(philo))
		return (NULL);
	while (!is_finished(philo))
	{
		if (!wait_can_eat(philo))
			return (NULL);
		if (!routine(philo))
			return (NULL);
	}
	return (NULL);
}

void	*philos_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	set_int_mutex(&philo->mtx, &philo->is_ready, 1);
	if (!wait_start(philo))
		return (NULL);
	while (!is_finished(philo))
	{
		if (!routine(philo))
			return (NULL);
	}
	return (NULL);
}
