#include "../philo.h"

static int	threads_creation(t_prog *prog)
{
	int		i;
	void	*(*routine_fct)(void *);

	i = -1;
	routine_fct = NULL;
	if (prog->arg.nb_philo == 1)
		routine_fct = &alone_routine;
	else if (prog->arg.nb_philo % 2 == 0)
		routine_fct = &even_routine;
	else
		routine_fct = &philos_routine;
	while (++i < prog->arg.nb_philo)
	{
		if (pthread_create(&prog->philos[i].th_id, NULL, \
			routine_fct, &prog->philos[i]) != 0)
			return (set_int_mutex(&prog->mtx, &prog->error, 1), 0);
	}
	if (pthread_create(&prog->monitoring_th, NULL, \
		&monitoring_routine, prog) != 0)
		return (set_int_mutex(&prog->mtx, &prog->error, 1), 0);
	return (1);
}

static int	join_threads(t_prog *prog)
{
	int	i;

	i = -1;
	while (++i < prog->arg.nb_philo)
	{
		if (pthread_join(prog->philos[i].th_id, NULL) != 0)
			return (set_int_mutex(&prog->mtx, &prog->error, 1), 0);
	}
	if (pthread_join(prog->monitoring_th, NULL) != 0)
		return (set_int_mutex(&prog->mtx, &prog->error, 1), 0);
	return (1);
}

int	run(t_prog *prog)
{
	update_time(prog);
	if (!threads_creation(prog))
		return (print_error(THREAD_ERROR, 0));
	if (!join_threads(prog))
		return (print_error(JOIN_ERROR, 0));
	return (1);
}

