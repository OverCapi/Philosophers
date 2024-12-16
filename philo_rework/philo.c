#include "philo.h"

int	main(int argc, char **argv)
{
	t_prog	prog;

	memset(&prog, 0, sizeof(t_prog));
	if (!parse_arg(&prog.arg, argc, argv))
		return (1);
	if (!init(&prog))
		return (1);
	run(&prog);
	clean(&prog);
	return (0);
}
