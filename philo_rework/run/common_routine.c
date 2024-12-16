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
	if (!print_status(philo, FORK_STATUS))
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
	if (!print_status(philo, EAT_STATUS))
		return (0);
	ft_usleep(philo->prog->arg.time_to_eat * 1000);
	pthread_mutex_lock(&philo->mtx);
	philo->last_time_eat = get_time_since_start(philo->prog);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->mtx);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (1);
}

static int	sleep_routine(t_philo *philo)
{
	if (!print_status(philo, SLEEP_STATUS))
		return (0);
	ft_usleep(philo->prog->arg.time_to_sleep * 1000);
	return (1);
}

static int	think_routine(t_philo *philo)
{
	if (!print_status(philo, THINK_STATUS))
		return (0);
	ft_usleep(100); // fix mb
	return (1);
}

int	routine(t_philo *philo)
{
	eat_routine(philo);
	if (is_finished(philo))
		return (0);
	sleep_routine(philo);
	if (is_finished(philo))
		return (0);
	think_routine(philo);
	return (1);
}
