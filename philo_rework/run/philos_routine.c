#include "../philo.h"

static int	get_fork(t_philo *philo, int fork_nb)
{
	if (fork_nb == 0)
	{
		if (philo->index % 2 == 0)
			pthread_mutex_lock(philo->left_fork);
		else
			pthread_mutex_lock(philo->right_fork);
	}
	else
	{
		if (philo->index % 2 == 0)
			pthread_mutex_lock(philo->right_fork);
		else
			pthread_mutex_lock(philo->left_fork);
	}
	if (!print_status(philo, FORK_STATUS, 0))
	{
		pthread_mutex_unlock(philo->left_fork); // mb issue
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	return (1);
}

static int	eat_routine(t_philo *philo)
{
	if (!get_fork(philo, 0))
		return (0);
	if (!get_fork(philo, 1))
		return (0);
	if (!print_status(philo, EAT_STATUS, 0))
		return (0);
	ft_usleep(philo->prog->arg.time_to_eat * 1000);
	pthread_mutex_lock(&philo->mtx);
	philo->last_time_eat = get_time_from_start(philo->prog);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->mtx);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (1);
}

void	*philos_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	set_int_mutex(&philo->mtx, &philo->is_ready, 1);
	if (!wait_start(philo))
		return (NULL);
	while (!is_finished(philo->prog))
	{
		eat_routine(philo);
		if (is_finished(philo->prog))
			break ;
		sleep(philo);
		if (is_finished(philo->prog))
			break ;
		think(philo);
	}
	return (NULL);
}
