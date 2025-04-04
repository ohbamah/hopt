#include "../includes/hopt.h"
#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct opt
{
	/*BOOL*/char	flood;
			int		count;
			char*	test[3];
			int		pids[6];
			char*	name;
}	t_opt;

char**	copy_av(int ac, char** av)
{
	char**	ret = malloc((ac + 1) * sizeof(char*));
	ret[ac] = NULL;
	for (int i = 0 ; i < ac ; ++i)
		ret[i] = strdup(av[i]);
	return (ret);
}

void	tests(int ntest, int ac, char*** av, t_opt* options, char endofarg, char undef, char redef, char overwrite)
{
	char**	_av = *av;
	char** tmp = copy_av(ac, *av);

	printf("\nTest %d: EOA:%s undef:%s redef:%s(%d)\n", ntest, endofarg == 1 ? "yes" : "no", undef == 1 ? "yes" : "no", redef == 1 ? "yes" : "no", overwrite);
	hopt_reset();
	memset(options, 0, sizeof(t_opt));
	if (endofarg)
		hopt_end_on_arg();
	if (undef)
		hopt_allow_undef();
	if (redef)
		hopt_allow_redef(overwrite);
	int optc = hopt(ac, _av);
	printf("arg count to skip %d\n", optc);
	if (optc == -1)
		printf("Error : %s\n", hopt_strerror());
	else
		printf("flood %d\ncount %d\ntest %s %s %s\npids %d %d %d %d %d %d\nname %s\n", options->flood, options->count, options->test[0], options->test[1], options->test[2], options->pids[0], options->pids[1], options->pids[2], options->pids[3], options->pids[4], options->pids[5], options->name);

	*av = tmp;
}

int	main(int ac, char** av)
{
	t_opt	options;

	memset(&options, 0, sizeof(options));
	hopt_add_option("c=-count",		1,	&options.count, HOPT_TYPE_INT);
	hopt_add_option("f=-flood=l",	0,	&options.flood);
	hopt_add_option("-test",		4,	&options.test, HOPT_TYPE_STR);
	hopt_add_option("p=-pid=-pids",	6,	&options.pids, HOPT_TYPE_INT);
	hopt_add_option("n=-name",		1,	&options.name, HOPT_TYPE_STR);

	printf("CMD=");
	for (int i = 0 ; i < ac ; ++i)
		printf("%s ", av[i]);
	printf("\n");
	tests(0, ac, &av, &options, 0, 0, 0, 0);
	tests(1, ac, &av, &options, 1, 0, 0, 0);
	tests(2, ac, &av, &options, 0, 1, 0, 0);
	tests(3, ac, &av, &options, 0, 0, 1, 0);
	tests(4, ac, &av, &options, 0, 0, 1, 1);
	tests(5, ac, &av, &options, 1, 1, 0, 0);
	tests(6, ac, &av, &options, 1, 0, 1, 0);
	tests(7, ac, &av, &options, 1, 0, 1, 1);
	tests(8, ac, &av, &options, 0, 1, 1, 0);
	tests(9, ac, &av, &options, 0, 1, 1, 1);
	tests(10, ac, &av, &options, 1, 1, 1, 1);
	hopt_free();
	return (0);
}
