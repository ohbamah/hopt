#include "__hopt_.h"
#include "hopt.h"

// re-SORT av tab strings
int
SORT(int ac, /*const*/ char** av, t_hopt_sort* head)
{
	if (!head)
		return (0);
	char**			ref;
	t_hopt_sort*	tmp = head;
	unsigned int	i = 1;
	int				count = 0;

	ref = malloc((ac + 1) * sizeof(char*));
	if (!ref)
	{
		hopt_nerr = HOPT_MALLOCF;
		return (-1);
	}
	ref[0] = av[0];
	while (tmp)
	{
		ref[i++] = av[tmp->index];
		for (unsigned int j = 1 ; j <= tmp->argc ; ++j)
			ref[i++] = av[tmp->index + j];
		count += tmp->argc + 1;
		tmp = tmp->next;
	}
	tmp = head;
	for (unsigned int j = 1 ; j < (unsigned int)ac ; ++j)
	{
		if (tmp)
		{
			if (j != tmp->index)
				ref[i++] = av[j];
			else
			{
				for (unsigned int g = 0 ; g < tmp->argc /*'+1' for tmp->index*/ ; ++g)
					++j;
				tmp = tmp->next;
			}
		}
	}
	ref[i] = NULL;
	hopt_free_lstsort(head);
	i = 1;
	for ( ; ref[i] ; ++i)
		av[i] = ref[i];
	free(ref);
	return (count);
}

static inline
void
FINDER_ERROR(t_hopt* restrict h, int errcode, unsigned int i, int j)
{
	memset(&hopt_cerr, 0, sizeof(hopt_cerr));
	h->f.error = TRUE;
	hopt_nerr = errcode;
	if (h->f.strso == TRUE)
		strncpy(hopt_cerr, &h->av[i][j], 1);
	else
	{
		if (strlen(&h->av[i][1]) < sizeof(hopt_cerr))
			strncpy(hopt_cerr, &h->av[i][1], strlen(&h->av[i][1]));
		else
			strncpy(hopt_cerr, &h->av[i][1], 15);
	}
}

static inline
void
FINDER_WRITE_TYPE(t_hopt* restrict h, unsigned int /*av index*/ idx, unsigned int j, unsigned int opt_idx)
{
	char* dup = strdup(h->av[idx]);	// Store in hopt_mallocd [old "hopt_addrs"]
	if (hopt_maps[opt_idx]->type == HOPT_TYPE_STR)
	{
		hopt_mallocd[hopt_c_mallocd + j] = dup;
		memcpy(hopt_maps[opt_idx]->mem + j * sizeof(char*), &dup, sizeof(char*));
	}
	else if (hopt_maps[opt_idx]->type == HOPT_TYPE_CHAR)
	{
		char	at = atoi(dup);
		*((char*)(hopt_maps[opt_idx]->mem) + j) = at;
	}
	else if (hopt_maps[opt_idx]->type == HOPT_TYPE_SHORT)
	{
		short	at = atoi(dup);
		*((short*)(hopt_maps[opt_idx]->mem) + j) = at;
	}
	else if (hopt_maps[opt_idx]->type == HOPT_TYPE_INT)
	{
		int	at = atoi(dup);
		*((int*)(hopt_maps[opt_idx]->mem) + j) = at;
	}
	else if (hopt_maps[opt_idx]->type == HOPT_TYPE_LONG)
	{
		long long	all = atoll(dup);
		*((long long*)(hopt_maps[opt_idx]->mem) + j) = all;
	}
	else if (hopt_maps[opt_idx]->type == HOPT_TYPE_FLOAT)
	{
		float	af = (float)atof(dup);
		*((float*)(hopt_maps[opt_idx]->mem) + j) = af;
	}
	else if (hopt_maps[opt_idx]->type == HOPT_TYPE_DOUBLE)
	{
		double	af = atoi(dup);
		*((double*)(hopt_maps[opt_idx]->mem) + j) = af;
	}
	if (hopt_maps[opt_idx]->type != HOPT_TYPE_STR)
		free(dup);
}

static
unsigned int
FINDER_WRITE(t_hopt* restrict h, unsigned int /*av index*/ idx, unsigned int /*av[idx] index*/ c, unsigned int opt_idx)
{
	unsigned int	j 			= 0;
	unsigned int	tmp 		= idx;

	if (h->oac > 0)
	{
		if (hopt_maps[opt_idx]->type == HOPT_TYPE_STR)
			hopt_mallocd = realloc(hopt_mallocd, (hopt_c_mallocd + h->oac) * sizeof(void*));
		++idx;
		while (h->av[idx] && j < h->oac)
		{

			if ((!h->av[idx + 1] && j < h->oac - 1U) || (h->av[idx][0] == '-' && strnlen(h->av[idx], 2) > 1 && !isdigit(h->av[idx][1])))
				FINDER_ERROR(h, HOPT_MISSOARGC, tmp, c);
			FINDER_WRITE_TYPE(h, idx, j, opt_idx);
			++j;
			++idx;
			++h->f.addrs_idx;
			++h->n_parsed;
		}
		--idx;
		if (hopt_maps[opt_idx]->type == HOPT_TYPE_STR)
			hopt_c_mallocd += j;
	}
	else
		++(*((BOOL*)hopt_maps[opt_idx]->mem));
	return (idx);
}

void
FINDER(t_hopt* restrict h)
{
	unsigned int	i;
	int				j;
	unsigned int	n;
	unsigned int	m;

	i = 1;
	while (i < (unsigned int)h->ac && h->f.error == FALSE)
	{
		int avi_size = strlen(&h->av[i][1]);
		BOOL is_an_option = h->av[i][0] == '-' && strnlen(&h->av[i][0], 2) > 1;
		if (is_an_option)
		{
			h->f.strso = FALSE;
			if (h->av[i][1] != '-' && strnlen(&h->av[i][1], 2) > 1)
				h->f.strso = TRUE;
			j = 1;
			while (h->av[i] && h->av[i][j] && is_an_option && h->f.error == FALSE)
			{
				n = 0;
				h->f.found = FALSE;
				h->offset = 0;
				while (n < hopt_c_maps && h->f.found == FALSE && h->f.error == FALSE)
				{
					h->oac = hopt_maps[n]->argc;
					char** alias = strsplit(hopt_maps[n]->names, '=');//FINDER_ALIAS(h, n);
					if (!alias)
					{
						hopt_nerr = HOPT_MALLOCF;
						h->f.error = TRUE;
						break ;
					}
					m = 0;
					while (alias[m])
					{
						if (h->f.found == FALSE && h->f.error == FALSE)
						{
							h->f.last_i = i;
							if (h->f.strso == FALSE && !strcmp(&h->av[i][1], alias[m]))
							{
								if (h->flags[n] == FALSE || (hopt_redef_allowed == TRUE && hopt_redef_overwrt == TRUE))
									i = FINDER_WRITE(h, i, 1, n);
								if (h->flags[n] == FALSE)
									++h->n_parsed;
								h->f.found = TRUE;
								j = avi_size;
							}
							else if (h->f.strso == TRUE && h->av[i][j] == alias[m][0])
							{
								if (h->oac > 0 && h->av[i][j + 1] != '\0')
									FINDER_ERROR(h, HOPT_BADSORDER, i, j);
								if (h->flags[n] == FALSE || (hopt_redef_allowed == TRUE && hopt_redef_overwrt == TRUE))
									FINDER_WRITE(h, i, j, n);
								if (h->flags[n] == FALSE && j == 1)
									++h->n_parsed;
								if (h->av[i][j + 1] == '\0')
									i += h->oac;
								h->f.found = TRUE;
							}
						}
						free(alias[m]);
						++m;
					}
					if (h->flags[n] == TRUE && h->f.found == TRUE && hopt_redef_allowed == FALSE)
						FINDER_ERROR(h, HOPT_REDEFINED, h->f.last_i, j);
					if (h->f.found == TRUE && h->f.error == FALSE && (h->flags[n] == FALSE || (hopt_redef_allowed == TRUE && hopt_redef_overwrt == TRUE && h->flags[n] == TRUE)))
					{
						if (hopt_disable_sort_v == FALSE)
						{
							if (h->f.head == NULL)
							{
								h->f.head = hopt_new_node(h->f.last_i, h->oac);
								h->f.last_node = h->f.head;
							}
							else
								hopt_add_back(&h->f.last_node, hopt_new_node(h->f.last_i, h->oac));
						}
						h->flags[n] = TRUE;
					}
					free(alias);
					++n;
				}
				if (h->f.found == FALSE && h->f.error == FALSE && hopt_undef_allowed == FALSE)
					FINDER_ERROR(h, HOPT_UNDEFINED, i, j);
				is_an_option = h->av[i][0] == '-' && strnlen(&h->av[i][0], 2) > 1;
				++j;
			}
		}
		else if (hopt_end_on_arg_v == TRUE)
			break ;
		++i;
	}

}
