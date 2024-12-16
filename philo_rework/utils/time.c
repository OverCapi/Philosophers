#include "../philo.h"

static size_t	get_time(int unit)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	if (unit == 0)
		return (time.tv_sec * 1000 + time.tv_usec / 1000);
	else if (unit == 1)
		return (time.tv_sec * 1000000 + time.tv_usec);
	else
		return (0);
}

void	ft_usleep(size_t time_us)
{
	size_t	start_time;
	size_t	remaining;
	size_t	tmp;

	start_time = get_time(1);
	remaining = time_us;
	while (remaining > 0)
	{
		remaining = get_time(1) - start_time;
		if (time_us - remaining <= 1000)
		{
			while (remaining != 0)
			{
				tmp = get_time(1) - start_time;
				if (tmp >= time_us)
					remaining = 0;
				else
					remaining = time_us - tmp;
				usleep(100);
			}
		}
		else
			usleep((time_us - remaining) / 2);
	}
}

size_t	get_time_since_start(t_prog *prog)
{
	size_t	current_time;

	current_time = get_time(0);
	pthread_mutex_lock(&prog->mtx);
	if (prog->start_time == 0)
		prog->start_time = current_time;
	else
		current_time -= prog->start_time;
	pthread_mutex_unlock(&prog->mtx);
	return (current_time);
}

void	update_time(t_prog *prog)
{
	size_t	current_time;

	current_time = get_time(0);
	pthread_mutex_lock(&prog->mtx);
	if (prog->start_time == 0)
		prog->start_time = current_time;
	else
		current_time -= prog->start_time;
	prog->time = current_time;
	pthread_mutex_unlock(&prog->mtx);
}
