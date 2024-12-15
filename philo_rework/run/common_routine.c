#include "../philo.h"

int	sleep_routine(t_philo *philo)
{
	if (!print_status(philo, SLEEP_STATUS, 0))
		return (0);
	ft_usleep(philo->prog->arg.time_to_sleep * 1000);
	return (1);
}

int	think_routine(t_philo *philo)
{
	if (!print_status(philo, THINK_STATUS, 0))
		return (0);
	ft_usleep(100); // fix mb
	return (1);
}
