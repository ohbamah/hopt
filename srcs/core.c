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

		// Allocation
	ref = malloc((ac + 1) * sizeof(char*));
	if (!ref)
	{
		hopt_nerr = HOPT_MALLOCF;
		return (-1);
	}
	ref[0] = av[0];
		// Sort option arguments
	while (tmp)
	{
		ref[i++] = av[tmp->index];
		if (strchr(av[tmp->index], '=') == NULL)
		{
			for (unsigned int j = 1 ; j <= tmp->argc ; ++j)
				ref[i++] = av[tmp->index + j];
			count += tmp->argc + 1;
		}
		else
		{
			tmp->argc = 0;
			++count;
		}
		tmp = tmp->next;
	}
	tmp = head;
		// Sort non-option arguments
	for (unsigned int j = 1 ; j < (unsigned int)ac && i < (unsigned int)ac ; ++j)
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
	for ( ; ref[i] && av[i] ; ++i)
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
	if (i_hopt_auto_help_v == TRUE && (i_hopt_help_flagsw & errcode))
		printf("%s\n", hopt_help_menu(i_hopt_cmd_name));
}

static inline
void
FINDER_WRITE_TYPE(char* arg, unsigned int j, unsigned int opt_idx)
{
	long long	at = 0LL;
	double		ad = 0.0;
	switch (i_hopt_maps[opt_idx].flag)
	{
		case HOPT_TYPE_STR:
			memcpy(i_hopt_maps[opt_idx].mem + j * sizeof(char*), &arg, sizeof(char*));
			break;
		case HOPT_TYPE_CHAR:
			at = atoi(arg);
			*((char*)(i_hopt_maps[opt_idx].mem) + j) = (char)at;
			break;
		case HOPT_TYPE_SHORT:
			at = atoi(arg);
			*((short*)(i_hopt_maps[opt_idx].mem) + j) = (short)at;
			break;
		case HOPT_TYPE_INT:
			at = atoi(arg);
			*((int*)(i_hopt_maps[opt_idx].mem) + j) = (int)at;
			break;
		case HOPT_TYPE_LONG:
			at = atoll(arg);
			*((long long*)(i_hopt_maps[opt_idx].mem) + j) = at;
			break;
		case HOPT_TYPE_FLOAT:
			ad = (float)atof(arg);
			*((float*)(i_hopt_maps[opt_idx].mem) + j) = (float)ad;
			break;
		case HOPT_TYPE_DOUBLE:
			ad = (double)atof(arg);
			*((double*)(i_hopt_maps[opt_idx].mem) + j) = ad;
			break;
	}
}

static
int
FINDER_WRITE(t_hopt* hopt_restrict h, unsigned int /*av index*/ idx, unsigned int /*av[idx] index*/ c, unsigned int opt_idx)
{
	unsigned int	j 			= 0;
	unsigned int	tmp 		= idx;
	BOOL			found		= FALSE;

	if (i_hopt_maps[opt_idx].mandatory == TRUE && i_hopt_flags[opt_idx] == FALSE)
		++h->f.mandatory_count;
	if ((i_hopt_maps[opt_idx].flag & 0xF) != HOPT_FLCB)
	{
		if (h->oac == 1)
		{
			char*	value = strchr(h->av[idx], '=');
			if (value)
			{
				FINDER_WRITE_TYPE(&value[1], j, opt_idx);
				found = TRUE;
			}
		}
		if (h->oac >= 1 && found == FALSE)
		{
			++idx;
			if (!h->av[idx])
				FINDER_ERROR(h, HOPT_MISSOARGC, idx - 1, c);
			while (h->av[idx] && j < h->oac) //? && h->f.error == FALSE
			{
				if ((!h->av[idx + 1] && j < h->oac - 1U) || (h->av[idx][0] == '-' && strnlen(h->av[idx], 2) > 1 && !isdigit(h->av[idx][1])))
					FINDER_ERROR(h, HOPT_MISSOARGC, tmp, c);
				FINDER_WRITE_TYPE(h->av[idx], j, opt_idx);
				++j;
				++idx;
				++h->f.addrs_idx;
				++h->n_parsed;
			}
			--idx;
		}
		else if (h->oac == 0)
			++(*((BOOL*)i_hopt_maps[opt_idx].mem));
	}
	else
	{
		if ((i_hopt_maps[opt_idx].cb)(h->oac, &h->av[idx], i_hopt_maps[opt_idx].cb_arg) == -1)
			return (-1);
	}
	return (idx);
}

static
BOOL
FINDER_LONG_CMP(const char* av_i, char* alias)
{
	char*	_strchr = strchr(av_i, '=');
	if (!_strchr)
		return (!strcmp(av_i, alias));
	return (strlen(av_i) - strlen(_strchr) == strlen(alias) && av_i[strlen(alias)] == '=');
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
			while (h->av[i] && (h->av[i][j] && h->av[i][j] != '=') && is_an_option && h->f.error == FALSE)
			{
				n = 0;
				h->f.found = FALSE;
				h->offset = 0;
				while (n < i_hopt_c_maps && h->f.found == FALSE && h->f.error == FALSE)
				{
					h->oac = i_hopt_maps[n].argc;
					char** alias = strsplit(i_hopt_maps[n].names, '=');//FINDER_ALIAS(h, n);
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
							if (h->f.strso == FALSE && FINDER_LONG_CMP(&h->av[i][1], alias[m]))/*!strcmp(&h->av[i][1], alias[m]))*/
							{
								int	tmp_i = i;
								if (i_hopt_flags[n] == FALSE || (i_hopt_redef_allowed == TRUE && i_hopt_redef_overwrt == TRUE))
								{
									tmp_i = FINDER_WRITE(h, i, 1, n);
									if (tmp_i == -1)
										FINDER_ERROR(h, HOPT_CBERROR, i, 1);
									i = (unsigned int)tmp_i;
								}
								if (i_hopt_flags[n] == FALSE)
									++h->n_parsed;
								h->f.found = TRUE;
								j = avi_size;
							}
							else if (h->f.strso == TRUE && h->av[i][j] == alias[m][0])
							{
								if (h->oac > 0 && (h->av[i][j + 1] != '\0' && (h->av[i][j + 1] != '=' && h->oac == 1)))
									FINDER_ERROR(h, HOPT_BADSORDER, i, j);
								if (i_hopt_flags[n] == FALSE || (i_hopt_redef_allowed == TRUE && i_hopt_redef_overwrt == TRUE))
									if (FINDER_WRITE(h, i, j, n) == -1)
										FINDER_ERROR(h, HOPT_CBERROR, i, j);
								if (i_hopt_flags[n] == FALSE && j == 1)
									++h->n_parsed;
								if (h->av[i][j + 1] == '\0')// || (h->av[i][j + 1] == '=' && h->oac == 1))
									i += h->oac;
								h->f.found = TRUE;
							}
						}
						free(alias[m]);
						++m;
					}
					if (i_hopt_flags[n] == TRUE && h->f.found == TRUE && i_hopt_redef_allowed == FALSE)
						FINDER_ERROR(h, HOPT_REDEFINED, h->f.last_i, j);
					if (h->f.found == TRUE && h->f.error == FALSE && (i_hopt_flags[n] == FALSE || (i_hopt_redef_allowed == TRUE && i_hopt_redef_overwrt == TRUE && i_hopt_flags[n] == TRUE)))
					{
						if (hopt_g_disable_sort == FALSE)
							hopt_add_back(&h->f.head, hopt_new_node(h->f.last_i, h->oac));
						i_hopt_flags[n] = TRUE;
					}
					free(alias);
					++n;
				}
				if (h->f.found == FALSE && h->f.error == FALSE && i_hopt_undef_allowed == FALSE)
					FINDER_ERROR(h, HOPT_UNDEFINED, i, j);
				else if (h->av[i])
					is_an_option = h->av[i][0] == '-' && strnlen(&h->av[i][0], 2) > 1;
				++j;
			}
		}
		else if (i_hopt_end_on_arg_v == TRUE && hopt_current_state == hopt_c_states)
			break ;
		else if (hopt_current_state < hopt_c_states)
		{
			for (unsigned int cmt = 1 ; cmt <= hopt_c_states ; ++cmt)
			{
				if (!strcmp(h->av[i], hopt_state[cmt]._hopt_cmd_name))
				{
					hopt_current_state = cmt;
					i_hopt_flags = calloc(hopt_state[cmt]._hopt_c_maps, sizeof(BOOL));
					break;
				}
			}
		}
		++i;
	}
}

void
__hopt_find_missing_mandatory(t_hopt* hopt_restrict h)
{
	int				f = FALSE;
	char**			s;
	unsigned int	size;

	for (unsigned int k = 0 ; k < hopt_c_states ; ++k)
	{
		t_hopt_state*	tmp = &hopt_state[hopt_c_states];
		for (unsigned int i = 0, j = 0 ; i < tmp->_hopt_c_maps ; ++i)
		{
			if (tmp->_hopt_maps[i].mandatory == TRUE)
			{
				if (f == TRUE)
				{
					hopt_nerr = HOPT_MISSOPT;
					s = strsplit(tmp->_hopt_maps[i].names, '=');
					size = strlen(s[0]);
					if (size < sizeof(hopt_cerr))
						strncpy(hopt_cerr, s[0], size);
					else
						strncpy(hopt_cerr, s[0], 15);
					if (tmp->_hopt_auto_help_v == TRUE && (tmp->_hopt_help_flagsw & HOPT_MISSOPT))
						printf("%s\n", hopt_help_menu(tmp->_hopt_cmd_name));
					free2((void**)s);
					return ;
				}
				++j;
			}
			if (j == h->f.mandatory_count)
				f = TRUE;
		}
	}
}

inline
int
__hopt_intern_print_help_menu(int __a, char** __b, char* cmd)
{
	(void)__a;
	(void)__b;
	hopt_help_has_been_called_v = TRUE;
	hopt_print_help_menu(cmd);
	return (0);
}

static inline
void
__hopt_genhm_group(t_hopt_state* hopt_restrict state, char** last_group, int i, unsigned int lenmax)
{
	*last_group = state->_hopt_maps[i].group;
	int		buffersize = lenmax + strlen(*last_group) + 10;
	char*	tmp = state->_hopt_help_menu_str;
	char*	line = malloc((buffersize + 1) * sizeof(char));
	line[buffersize] = '\0';
	snprintf(line, buffersize, "\e[1m\n  %-*s\e[0m\n", (int)lenmax, *last_group);
	state->_hopt_help_menu_str = hopt_strjoin(state->_hopt_help_menu_str, line);
	free(tmp);
	free(line);
}

void
__hopt_generate_help_menu(t_hopt_state* hopt_restrict state)
{
	char*	endonarg_str = "";
	char*	tmp;

	if (state->_hopt_end_on_arg_v == FALSE)
		endonarg_str = "[OPTIONS...]";
	state->_hopt_help_menu_str =
		hopt_strvajoin(10,
			"Usage: ", basename(hopt_program_path),
			state->_hopt_cmd_name ? " " : "",
			state->_hopt_cmd_name ? state->_hopt_cmd_name : "",
			" [OPTIONS...] ", "ARGS... ", endonarg_str, "\n",
			hopt_g_program_desc == NULL ? "" : hopt_g_program_desc, "\n\n");
	unsigned int	lenmax = 0;
	for (unsigned int i = 0 ; i < state->_hopt_c_maps ; ++i)
	{
		if (state->_hopt_maps[i].desc != NULL)
		{
			char**	splitted = strsplit(state->_hopt_maps[i].names, '=');
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
	char*	last_group = NULL;
	for (unsigned int i = 0 ; i < state->_hopt_c_maps ; ++i)
	{
		if (last_group != state->_hopt_maps[i].group)	// just compare address
			__hopt_genhm_group(state, &last_group, i, lenmax);
		if (state->_hopt_maps[i].desc != NULL)
		{
			char**	splitted = strsplit(state->_hopt_maps[i].names, '=');
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
			int		buffersize = lenmax + strlen(state->_hopt_maps[i].desc) + 8;
			char*	line = malloc((buffersize + 1) * sizeof(char));
			snprintf(line, buffersize, "  %-*s\t%s\n", (int)lenmax, aliases, state->_hopt_maps[i].desc);
			line[buffersize] = '\0';
			tmp = state->_hopt_help_menu_str;
			state->_hopt_help_menu_str = hopt_strjoin(state->_hopt_help_menu_str, line);
			free(tmp);
			free(aliases);
			free(line);
		}
	}
}
