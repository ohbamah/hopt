#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct opt
{
	/*BOOL*/char	flood;
			int		count;
			char*	test;
			int		pids;
			char*	name;
			char*	arg;
}	t_opt;

static struct argp_option options[] =
{
	{ "count", 'c', "NUM", 0, "Set count value" },
	{ "flood", 'f', 0, 0, "Enable flooding" },
	{ "test", 1, "STR", 0, "Test option" },
	{ "pid", 'p', "NUM", 0, "Set PID value" },
	{ "name", 'n', "STR", 0, "Set name value" },
	{ 0 }
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
 	t_opt *opts = state->input;
 	switch (key)
	{
		case 'c': opts->count = atoi(arg);	break;
		case 'f': opts->flood = 1;			break;
		case   1: opts->test = arg;			break;
		case 'p': opts->pids = atoi(arg); 	break;
		case 'n': opts->name = arg;			break;
		case ARGP_KEY_ARG:opts->arg = arg;	break;
		case ARGP_KEY_END:					break;
		default: return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = { options, parse_opt, 0, "Test program for argp." };

unsigned long
microsecond(struct timeval* __restrict__ tv)
{
	return (tv->tv_sec * 1000000 + tv->tv_usec);
}

int main(int argc, char **argv)
{
	struct timeval	tv = {0};
	gettimeofday(&tv, NULL);
	unsigned long	start_at = microsecond(&tv);

	t_opt opts = {0};
	argp_parse(&argp, argc, argv, 0, 0, &opts);

	gettimeofday(&tv, NULL);
	unsigned long	end_at = microsecond(&tv);
	printf("Duration : %ldμs\n\n", end_at - start_at);

	printf("count: %d\n", opts.count);
	printf("flood: %d\n", opts.flood);
	printf("test: %s\n", opts.test ? opts.test : "(null)");
	printf("pids: %d\n", opts.pids);
	printf("name: %s\n", opts.name ? opts.name : "(null)");
	return 0;
}