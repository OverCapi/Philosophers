#include "../philo.h"


int	is_finished(t_philo *philo)
{
	int	is_running;
	int	eat_count;

	is_running = get_int_mutex(&philo->prog->mtx, &philo->prog->is_running);
	if (get_int_mutex(&philo->mtx, &philo->is_dead) == 1)
		return (print_status(philo, DIED_STATUS, 1), 1);
	if (philo->prog->arg.max_eat != -1)
	{
		eat_count = get_int_mutex(&philo->mtx, &philo->eat_count);
		if (eat_count >= philo->prog->arg.max_eat)
			return (1);
	}
	return (!is_running);
}

int	wait_odd_can_eat(t_philo *philo)
{
	while (1)
	{
		if (!is_finished(philo))
			return (0);
		if (get_int_mutex(&philo->prog->mtx, &philo->prog->odd_philo_can_eat))
			return (1);
	}
	return (0);
}

int	print_status(t_philo *philo, char *status, int dead_status)
{
	static int	alr_dead = 0;
	size_t		time;

	if (dead_status == 1 && alr_dead != 0)
		return (0);
	time = get_size_t_mutex(&philo->prog->mtx, &philo->prog->time);
	if (is_finished(philo))
		return (0);
	pthread_mutex_lock(&philo->prog->write_perm);
	printf("%zu %d %s", time, philo->index, status);
	pthread_mutex_unlock(&philo->prog->write_perm);
	if (dead_status == 1 && alr_dead == 0)
		alr_dead = 1;
	return (1);
}

int	wait_start(t_philo *philo)
{
	while (1)
	{
		if (get_int_mutex(&philo->mtx, &philo->is_dead))
			return ((void)print_status(philo, DIED_STATUS, 1), 0);
		if (get_int_mutex(&philo->prog->mtx, &philo->prog->error))
			return (0);
		if (get_int_mutex(&philo->prog->mtx, &philo->prog->is_running))
			return (1);
	}
	return (0);
}
