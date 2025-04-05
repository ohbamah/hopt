#include "../includes/hopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

typedef struct opt
{
	/*BOOL*/char	flood;
			int		count;
			char*	test[4];
			int		pids[6];
			char*	name;
}	t_opt;

unsigned long
microsecond(struct timeval* __restrict__ tv)
{
	return (tv->tv_sec * 1000000 + tv->tv_usec);
}

int	cbtest(int ac, char** av, void* arg)
{
	printf("ac : %d\n", ac);
	for (int i = 0 ; i < ac ; ++i)
		printf("av[%d] = %s\n", i, av[i]);
	if (!strcmp(av[0], "-n") || !strcmp(av[0], "--name"))
	{
		printf("Option -n=--name found!\n");
		((t_opt*)arg)->name = "FOUND";
	}
	return (0);
}

int	main(int ac, char** av)
{
	struct timeval	tv = {0};
	gettimeofday(&tv, NULL);
	unsigned long	start_at = microsecond(&tv);

	t_opt	options = {0};

	//hopt_allow_redef(FALSE/TRUE);	//? Allow options redefinition (with overwriting or not)
	//hopt_allow_undef();			//? Allow undefined options (ignored)
	//hopt_end_on_arg();			//? End parsing while a non-option argument appears
	//hopt_disable_sort();			//? Disable AV sorting
	//hopt_auto_help();				//? Automatically show the help menu when an option is undefined (or miss argument count)
	//hopt_reset();					//? Reset all HOPT functionnalities (allow redef/undef and end_on_arg)
	hopt_add_option("c=-count",		1, HOPT_TYPE_INT,	&options.count);
	hopt_add_option("f=-flood=l",	0, 0,				&options.flood);
	hopt_add_option("-test",		4, HOPT_TYPE_STR,	&options.test);
	hopt_add_option("p=-pid=-pids",	6, HOPT_TYPE_INT,	&options.pids);
	hopt_add_option("n=-name",		1, HOPT_TYPE_STR,	&options.name);//cbtest, &options);

	hopt(ac, av);

	//printf("%s\n", hopt_help_menu());

	hopt_free();

	gettimeofday(&tv, NULL);
	unsigned long	end_at = microsecond(&tv);
	printf("Duration : %ldμs\n\n", end_at - start_at);

	char*	error = hopt_strerror();
	printf("%s\n", error);
	free(error);

	printf("count: %d\n", options.count);
	printf("flood: %d\n", options.flood);
	printf("test: %s %s %s %s\n", options.test[0], options.test[1], options.test[2], options.test[3]);
	printf("pids: %d %d %d %d %d %d\n", options.pids[0], options.pids[1], options.pids[2], options.pids[3], options.pids[4], options.pids[5]);
	printf("name: %s\n", options.name ? options.name : "(null)");

	for (int i = 0 ; i < ac ; ++i)
		printf("av[%d] = %s\n", i, av[i]);
	return (0);
}
