#include "../philo.h"

static int	wait_all_threads_ready(t_prog *prog)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < prog->arg.nb_philo)
		{
			if (get_int_mutex(&prog->mtx, &prog->error))
				return (0);
			if (!get_int_mutex(&prog->philos[i].mtx, &prog->philos[i].is_ready))
				break ;
		}
		if (i == prog->arg.nb_philo)
			return (1);
	}
	return (1);
}

static int	check_odd_can_eat(t_prog *prog)
{
	int	i;

	i = 0;
	while (i < prog->arg.nb_philo)
	{
		if (!get_int_mutex(&prog->philos[i].mtx, &prog->philos[i].eat_count))
			return (0);
		i += 2;
	}
	return (1);
}

static int	philos_dead(t_prog *prog)
{
	int		i;
	size_t	last_time_eat;
	size_t	current_time;

	i = -1;
	current_time = get_time_since_start(prog);
	while (++i < prog->arg.nb_philo)
	{
		last_time_eat = get_size_t_mutex(&prog->philos[i].mtx, \
			&prog->philos[i].last_time_eat);
		if (current_time - last_time_eat >= (size_t)(prog->arg.time_to_die + 5))
		{
			set_int_mutex(&prog->philos[i].mtx, &prog->philos[i].is_dead, 1);
			set_int_mutex(&prog->mtx, &prog->is_running, 0);
			return (1);
		}
	}
	return (0);
}

static int	check_philos_full(t_prog *prog)
{
	int	i;
	int	eat_count;

	i = -1;
	while (++i < prog->arg.nb_philo)
	{
		eat_count = get_int_mutex(&prog->philos[i].mtx, \
			&prog->philos[i].eat_count);
		if (eat_count < prog->arg.max_eat)
			return (0);
	}
	set_int_mutex(&prog->mtx, &prog->is_running, 0);
	return (1);
}

void	*monitoring_routine(void *arg)
{
	t_prog	*prog;
	int		alr_check;

	alr_check = 0;
	prog = (t_prog *)arg;
	if (!wait_all_threads_ready(prog))
		return (NULL);
	set_int_mutex(&prog->mtx, &prog->is_running, 1);
	while (1)
	{
		if (!alr_check && check_odd_can_eat(prog))
		{
			set_int_mutex(&prog->mtx, &prog->odd_philo_can_eat, 1);
			alr_check = 1;
		}
		update_time(prog);
		if (philos_dead(prog))
			return (NULL);
		if (prog->arg.max_eat != -1 && check_philos_full(prog))
			return (NULL);
	}
	return (NULL);
}
