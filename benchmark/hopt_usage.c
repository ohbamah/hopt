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
			char*	cb;
}	t_opt;

unsigned long
microsecond(struct timeval* __restrict__ tv)
{
	return (tv->tv_sec * 1000000 + tv->tv_usec);
}

int	cbtest(int ac, char** av, void* arg)
{
	if (!strcmp(av[0], "-b") || !strcmp(av[0], "--callback"))
	{
		printf("Option -b=--callback found!\n");
		((t_opt*)arg)->cb = av[1];
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
	//hopt_auto_help(FALSE/TRUE);	//? Automatically show the help menu when an option is undefined (or miss argument count)
	//hopt_reset();					//? Reset all HOPT functionnalities (allow redef/undef and end_on_arg)
	//hopt_program_description("Send ICMP ECHO_REQUEST packets to network hosts.");
	hopt_add_option("c=-count",		1, HOPT_TYPE_INT,	&options.count,	"Max packet to send");
	hopt_add_option("f=-flood=l",	0, 0,				&options.flood,	"Flood network");
	hopt_add_option("-test",		4, HOPT_TYPE_STR,	&options.test,	NULL);
	hopt_add_option("p=-pid=-pids",	6, HOPT_TYPE_INT,	&options.pids,	"Pid processes to kill");
	hopt_add_option("n=-name",		1, HOPT_TYPE_STR,	&options.name,	"");
	hopt_add_option("b=-callback",  1, HOPT_FLCB,		cbtest, &options,    "Just testing callback"); // With flag HOPT_FLCB it will call 'cbtest(1, "-b ..."/"--callback ...", &options)'
	hopt(ac, av);
	printf("%s\n", hopt_help_menu());
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
