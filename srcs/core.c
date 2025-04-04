/*
 * MIT License
 * 
 * Copyright (c) 2025 OhBama (Yann Manchon)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

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
FINDER_ERROR(t_hopt* hopt_restrict h, int errcode, unsigned int i, int j)
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
	if (hopt_auto_help_v == TRUE && (errcode == HOPT_UNDEFINED || errcode == HOPT_MISSOARGC))
		printf("%s\n", hopt_help_menu());
}

static inline
void
FINDER_WRITE_TYPE(t_hopt* hopt_restrict h, unsigned int /*av index*/ idx, unsigned int j, unsigned int opt_idx)
{
	char* dup = h->av[idx];
	long long	at = 0LL;
	double		ad = 0.0;
	switch (hopt_maps[opt_idx]->flag)
	{
		case HOPT_TYPE_STR:
			memcpy(hopt_maps[opt_idx]->mem + j * sizeof(char*), &dup, sizeof(char*));
			break;
		case HOPT_TYPE_CHAR:
			at = atoi(dup);
			*((char*)(hopt_maps[opt_idx]->mem) + j) = (char)at;
			break;
		case HOPT_TYPE_SHORT:
			at = atoi(dup);
			*((short*)(hopt_maps[opt_idx]->mem) + j) = (short)at;
			break;
		case HOPT_TYPE_INT:
			at = atoi(dup);
			*((int*)(hopt_maps[opt_idx]->mem) + j) = (int)at;
			break;
		case HOPT_TYPE_LONG:
			at = atoll(dup);
			*((long long*)(hopt_maps[opt_idx]->mem) + j) = at;
			break;
		case HOPT_TYPE_FLOAT:
			ad = (float)atof(dup);
			*((float*)(hopt_maps[opt_idx]->mem) + j) = (float)ad;
			break;
		case HOPT_TYPE_DOUBLE:
			ad = (double)atof(dup);
			*((double*)(hopt_maps[opt_idx]->mem) + j) = ad;
			break;
	}
}

static
int
FINDER_WRITE(t_hopt* hopt_restrict h, unsigned int /*av index*/ idx, unsigned int /*av[idx] index*/ c, unsigned int opt_idx, unsigned int __j)
{
	unsigned int	j 			= 0;
	unsigned int	tmp 		= idx;

	if (hopt_maps[opt_idx]->flag != HOPT_FLCB)
	{
		if (h->oac > 0)
		{
			++idx;
			if (!h->av[idx])
				FINDER_ERROR(h, HOPT_MISSOARGC, idx - 1, __j);
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
		}
		else
			++(*((BOOL*)hopt_maps[opt_idx]->mem));
	}
	else
	{
		if ((hopt_maps[opt_idx]->cb)(h->oac, &h->av[idx], hopt_maps[opt_idx]->cb_arg) == -1)
			return (-1);
	}
	return (idx);
}

void
FINDER(t_hopt* hopt_restrict h)
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
								int	tmp_i = i;
								if (h->flags[n] == FALSE || (hopt_redef_allowed == TRUE && hopt_redef_overwrt == TRUE))
								{
									tmp_i = FINDER_WRITE(h, i, 1, n, j);
									if (tmp_i == -1)
										FINDER_ERROR(h, HOPT_CBERROR, i, j);
									i = (unsigned int)tmp_i;
								}
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
									if (FINDER_WRITE(h, i, j, n, j) == -1)
										FINDER_ERROR(h, HOPT_CBERROR, i, j);
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
							hopt_add_back(&h->f.head, hopt_new_node(h->f.last_i, h->oac));
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

void
hopt_generate_help_menu(void)
{
	char*	endonarg_str = "";
	char*	tmp;

	if (hopt_end_on_arg_v == FALSE)
		endonarg_str = "[OPTIONS...]";
	hopt_help_menu_str =
		hopt_strvajoin(8,
			"Usage: ", basename(hopt_program_name), " [OPTIONS...] ", "ARGS... ", endonarg_str, "\n",\
			hopt_program_desc == NULL ? "" : hopt_program_desc, "\n\n");
	unsigned int	lenmax = 0;
	for (unsigned int i = 0 ; i < hopt_c_maps ; ++i)
	{
		if (hopt_maps[i]->desc != NULL)
		{
			char**	splitted = strsplit(hopt_maps[i]->names, '=');
			char*	aliases = hopt_strjoin("-", splitted[0]);
			free(splitted[0]);
			for (int j = 1 ; splitted[j] ; ++j)
			{
				tmp = aliases;
				aliases = hopt_strvajoin(3, aliases, ", -", splitted[j]);
				free(tmp);
				free(splitted[j]);
			}
			free(splitted);
			if (strlen(aliases) > lenmax)
				lenmax = strlen(aliases);
			free(aliases);
		}
	}
	for (unsigned int i = 0 ; i < hopt_c_maps ; ++i)
	{
		if (hopt_maps[i]->desc != NULL)
		{
			char**	splitted = strsplit(hopt_maps[i]->names, '=');
			char*	aliases = hopt_strjoin("-", splitted[0]);
			free(splitted[0]);
			for (int j = 1 ; splitted[j] ; ++j)
			{
				tmp = aliases;
				aliases = hopt_strvajoin(3, aliases, ", -", splitted[j]);
				free(tmp);
				free(splitted[j]);
			}
			free(splitted);
			int		buffersize = lenmax + strlen(hopt_maps[i]->desc) + 8;
			char*	line = malloc((buffersize + 1) * sizeof(char));
			snprintf(line, buffersize, "  %-*s\t%s\n", (int)lenmax, aliases, hopt_maps[i]->desc);
			line[buffersize] = '\0';
			tmp = hopt_help_menu_str;
			hopt_help_menu_str = hopt_strjoin(hopt_help_menu_str, line);
			free(tmp);
			free(aliases);
			free(line);
		}
	}
}
