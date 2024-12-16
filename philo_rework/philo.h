#ifndef PHILO_H
# define PHILO_H

# define ARG_ERROR "Usage : ./philo number_of_philosophers \
time_to_die \
time_to_eat \
time_to_sleep \
[number_of_times_each_philosopher_must_eat]\n"
# define ARG_VALUE_ERROR "Parameters must be greater or equal than 0\n"
# define ALLOC_ERROR "Memory allocation failed\n"
# define INIT_ERROR "Initialization failed\n"
# define THREAD_ERROR "Thread creation failed\n"
# define JOIN_ERROR "Thread join failed\n"

# define EAT_STATUS "is eating\n"
# define SLEEP_STATUS "is sleeping\n"
# define THINK_STATUS "is thinking\n"
# define FORK_STATUS "has taken a fork\n"
# define DIED_STATUS "died\n"

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>
# include <stdlib.h>

typedef struct s_prog	t_prog;

typedef struct s_arg
{
	int	nb_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	max_eat;
}	t_arg;

typedef struct s_philo
{
	pthread_t		th_id;
	int				index;
	size_t			last_time_eat;
	int				eat_count;
	int				is_dead;
	int				is_ready;
	pthread_mutex_t	mtx;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	t_prog			*prog;
}	t_philo;

typedef struct s_prog
{
	t_arg			arg;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	int				is_running;
	int				error;
	int				odd_philo_can_eat;
	size_t			start_time;
	size_t			time;
	pthread_t		monitoring_th;
	pthread_mutex_t	write_perm;
	pthread_mutex_t	mtx;
}	t_prog;

/*

AJOUTER DES MESSAGES ERREUR DANS INIT ETC ... ----------------------------------------------
MODIF MESSAGE DERREUR POUR LA SORTIE DERREUR

*/

/* UTILS */
// mutex_getters_setters.c
void	set_int_mutex(pthread_mutex_t *mtx, int *ptr, int value);
int		get_int_mutex(pthread_mutex_t *mtx, int *ptr);
void	set_size_t_mutex(pthread_mutex_t *mtx, size_t *ptr, size_t value);
size_t	get_size_t_mutex(pthread_mutex_t *mtx, size_t *ptr);

//routine_utils.c
int		is_finished(t_philo *philo);
int		print_status(t_philo *philo, char *status);
int		wait_start(t_philo *philo);
int		wait_odd_can_eat(t_philo *philo);

// time.c
size_t	get_time_since_start(t_prog *prog);
void	ft_usleep(size_t time_us);
void	update_time(t_prog *prog);

// print_error.c
int		print_error(char *msg, int status);

// ft_atoi_safe.c
int		ft_atoi_safe(const char *nptr);

/* PARSING */
// parsing.c
int		parse_arg(t_arg *arg, int argc, char **argv);

/* INIT */
// init.c
int		init(t_prog *prog);

/* RUN */
// common_routime.c
int		routine(t_philo *philo);
// philos_routine.c
void	*alone_routine(void *arg);
void	*even_routine(void *arg);
void	*philos_routine(void *arg);
void	*monitoring_routine(void *arg);
// run.c
int		run(t_prog *prog);

/* CLEAN */
// clean.c
void	destroy_mutex(t_prog *prog, int size);
void	clean_mutex(t_prog *prog);
void	clean_philos(t_prog *prog, int size);
void	clean(t_prog *prog);

#endif