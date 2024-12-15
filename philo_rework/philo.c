#include "philo.h"

int	main(int argc, char **argv)
{
	t_prog	prog;

	memset(&prog, 0, sizeof(t_prog));
	if (!parse_arg(&prog.arg, argc, argv))
		return (1);
	if (!init(&prog))
		return (1);
	if (!run(&prog))
		return (1);
	clean(&prog);
	return (0);
}
