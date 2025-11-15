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

// static inline
// void
// FINDER_ERROR(t_hopt* hopt_restrict h, int errcode, unsigned int i, int j)
// {
// 	memset(&hopt_cerr, 0, sizeof(hopt_cerr));
// 	h->f.error = TRUE;
// 	hopt_nerr = errcode;
// 	if (h->f.is_short_option == TRUE)
// 		strncpy(hopt_cerr, &h->av[i][j], 1);
// 	else
// 	{
// 		if (strlen(&h->av[i][1]) < sizeof(hopt_cerr))
// 			strncpy(hopt_cerr, &h->av[i][1], strlen(&h->av[i][1]));
// 		else
// 			strncpy(hopt_cerr, &h->av[i][1], 15);
// 	}
// 	if (i_hopt_auto_help_v == TRUE && (i_hopt_help_flagsw & errcode))
// 		printf("%s\n", hopt_help_menu(i_hopt_cmd_name));
// }

static inline
void
ERROR_SYSTEM(t_hopt* hopt_restrict h, int errcode, const char* option, unsigned int len)
{
	memset(&hopt_cerr, 0, HOPT_MAX_SSTR_SIZE);
	h->f.error = TRUE;
	hopt_nerr = errcode;
	if (strlen(option) < HOPT_MAX_SSTR_SIZE - 2)
		strncpy(hopt_cerr, option, len);
	else
		strncpy(hopt_cerr, option, HOPT_MAX_SSTR_SIZE - 2);
}

// static inline
// void
// FINDER_WRITE_TYPE(char* arg, unsigned int j, unsigned int opt_idx)
// {
// 	long long	at = 0LL;
// 	double		ad = 0.0;
// 	switch (i_hopt_maps[opt_idx].flag)
// 	{
// 		case HOPT_TYPE_STR:
// 			memcpy(i_hopt_maps[opt_idx].mem + j * sizeof(char*), &arg, sizeof(char*));
// 			break;
// 		case HOPT_TYPE_CHAR:
// 			at = atoi(arg);
// 			*((char*)(i_hopt_maps[opt_idx].mem) + j) = (char)at;
// 			break;
// 		case HOPT_TYPE_SHORT:
// 			at = atoi(arg);
// 			*((short*)(i_hopt_maps[opt_idx].mem) + j) = (short)at;
// 			break;
// 		case HOPT_TYPE_INT:
// 			at = atoi(arg);
// 			*((int*)(i_hopt_maps[opt_idx].mem) + j) = (int)at;
// 			break;
// 		case HOPT_TYPE_LONG:
// 			at = atoll(arg);
// 			*((long long*)(i_hopt_maps[opt_idx].mem) + j) = at;
// 			break;
// 		case HOPT_TYPE_FLOAT:
// 			ad = (float)atof(arg);
// 			*((float*)(i_hopt_maps[opt_idx].mem) + j) = (float)ad;
// 			break;
// 		case HOPT_TYPE_DOUBLE:
// 			ad = (double)atof(arg);
// 			*((double*)(i_hopt_maps[opt_idx].mem) + j) = ad;
// 			break;
// 	}
// }

static inline
void
TYPOS(t_hopt* hopt_restrict h, char* arg, unsigned int i, t_hopt_map* current_map)
{
	long long	at = 0LL;
	double		ad = 0.0;
	switch (i_hopt_maps[h->f.n].flag)
	{
		case HOPT_TYPE_STR:
			memcpy(current_map->mem + i * sizeof(char*), &arg, sizeof(char*));
			break;
		case HOPT_TYPE_CHAR:
			at = atoi(arg);
			*((char*)(current_map->mem) + i) = (char)at;
			break;
		case HOPT_TYPE_SHORT:
			at = atoi(arg);
			*((short*)(current_map->mem) + i) = (short)at;
			break;
		case HOPT_TYPE_INT:
			at = atoi(arg);
			*((int*)(current_map->mem) + i) = (int)at;
			break;
		case HOPT_TYPE_LONG:
			at = atoll(arg);
			*((long long*)(current_map->mem) + i) = at;
			break;
		case HOPT_TYPE_FLOAT:
			ad = (float)atof(arg);
			*((float*)(current_map->mem) + i) = (float)ad;
			break;
		case HOPT_TYPE_DOUBLE:
			ad = (double)atof(arg);
			*((double*)(current_map->mem) + i) = ad;
			break;
	}
}

// static
// int
// FINDER_WRITE(t_hopt* hopt_restrict h, unsigned int /*av index*/ idx, unsigned int /*av[idx] index*/ c, unsigned int opt_idx)
// {
// 	unsigned int	j 			= 0;
// 	unsigned int	tmp 		= idx;

// 	if (i_hopt_maps[opt_idx].mandatory == TRUE && i_hopt_flags[opt_idx] == FALSE)
// 		++h->f.mandatory_count;
// 	if ((i_hopt_maps[opt_idx].flag & 0xF) != HOPT_TYPE_CB)
// 	{
// 		if (h->oac == 1)
// 		{
// 			printf("1 ? - %s\n", h->av[idx]);
// 			char*	value = strchr(h->av[idx], '=');
// 			if (value)
// 			{
// 				FINDER_WRITE_TYPE(&value[1], j, opt_idx);
// 				return (idx);
// 			}
// 		}
// 		if (h->oac >= 1)
// 		{
// 			++idx;
// 			if (!h->av[idx])
// 				FINDER_ERROR(h, HOPT_MISSOARGC, idx - 1, c);
// 			while (h->f.error == FALSE && h->av[idx] && j < (unsigned int)h->oac)
// 			{
// 				printf("%d <=> %s\n", h->oac, h->av[idx]);
// 				if ((!h->av[idx + 1] && j < h->oac - 1U)) // || (h->av[idx][0] == '-' && strnlen(h->av[idx], 2) > 1 && !isdigit(h->av[idx][1]))
// 					FINDER_ERROR(h, HOPT_MISSOARGC, tmp, c);
// 				FINDER_WRITE_TYPE(h->av[idx], j, opt_idx);
// 				++j;
// 				++idx;
// 				++h->f.addrs_idx;
// 				++h->n_parsed;
// 			}
// 			// ++idx;
// 			printf("%d <=> %s\n", h->oac, h->av[idx]);
// 		}
// 		else if (h->oac == 0)
// 			++(*((BOOL*)i_hopt_maps[opt_idx].mem));
// 	}
// 	else
// 	{
// 		if ((i_hopt_maps[opt_idx].cb)(h->oac, &h->av[idx], i_hopt_maps[opt_idx].cb_arg) == -1)
// 			return (-1);
// 		h->n_parsed += h->oac;
// 		idx += h->oac;
// 	}
// 	return (idx);
// }

static
void
ANHIHILATOR(t_hopt* hopt_restrict h, const char* option, unsigned int len)
{
	unsigned int n = h->f.n;

	if (i_hopt_maps[n].mandatory && !i_hopt_flags[n])
		++h->f.mandatory_count;
	if ((i_hopt_maps[n].flag & 0xF) != HOPT_TYPE_CB)
	{
		if (h->oac == 1)
		{
			char*	value = strchr(option, '=');
			if (value && ++value)
			{
				TYPOS(h, value, 0, &i_hopt_maps[n]);
				return ;
			}
		}
		if (h->oac >= 1)
		{
			++h->f.i; // skip the current option
			++h->n_parsed;
			if (!h->av[h->f.i])
				ERROR_SYSTEM(h, HOPT_MISSOARGC, option, len);
			if (!h->f.error)
			{
				unsigned int i = 0;
				for /* each option arguments */
					(;
					 h->av[h->f.i] && i < (unsigned int)h->oac ;
					 ++i)
				{
					TYPOS(h, h->av[h->f.i], i, &i_hopt_maps[n]);
					++h->f.i;
					++h->n_parsed;
				}
				if (i < (unsigned int)h->oac)
					ERROR_SYSTEM(h, HOPT_MISSOARGC, option, len);
			}
			--h->f.i; // Because `h->f.i` is already skipped in the `for` loop
			--h->n_parsed;
		}
		else
			++(*((BOOL*)i_hopt_maps[n].mem));
	}
	else
	{
		if ((i_hopt_maps[n].cb)(h->oac, &h->av[h->f.i], i_hopt_maps[n].cb_arg) == -1)
			ERROR_SYSTEM(h, HOPT_CBERROR, option, len);
	}
	return ;
}

static
BOOL
is_valide_long_option(const char* av_i, char* alias)
{
	char*	_strchr = strchr(av_i, '=');
	if (!_strchr)
		return (!strcmp(av_i, alias));

	unsigned int	size = abs((int)(strlen(av_i) - strlen(_strchr)));
	return (!strncmp(av_i, alias, size) && strlen(alias) == size);
}

static
BOOL
is_valid_option(const char* alias, const char* option, unsigned int len)
{
	char*	option_value = strchr(option, '=');

	if (!option_value)
		return (!strncmp(option, alias, len) && strlen(alias) == len);
	
	int				value_size = strlen(option_value);
	unsigned int	size = abs(((int)len - value_size));
	return (!strncmp(option, alias, size) && strlen(alias) == size);
}

inline
void
__execute_subcommand_if_exists(unsigned int old_state_index)
{
	t_hopt_state*	state = &hopt_state[old_state_index];

	if (hopt_current_state > 0 && !state->_hopt_subcommand_executed && state->_hopt_subcommand_cb)
	{
		if (state->_hopt_subcommand_returns)
			*state->_hopt_subcommand_returns = state->_hopt_subcommand_cb(state->_hopt_subcommand_arg);
		else
			state->_hopt_subcommand_cb(state->_hopt_subcommand_arg);
		state->_hopt_subcommand_executed = TRUE;
	}
}

void
SLAVE(t_hopt* hopt_restrict h, const char* option, unsigned int len)
{
	if (i_hopt_flags[h->f.n] && !i_hopt_redef_allowed)
		ERROR_SYSTEM(h, HOPT_REDEFINED, option, len);
	else if ((!i_hopt_flags[h->f.n] || (i_hopt_flags[h->f.n] && i_hopt_redef_overwrt)))
	{
		if (!hopt_g_disable_sort)
			hopt_add_back(&h->f.head, hopt_new_node(h->f.last_i, h->oac));
		i_hopt_flags[h->f.n] = TRUE;
	}
}

void
EXECUTOR(t_hopt* hopt_restrict h, const char* option, unsigned int len)
{
	char**	aliases;
	char*	alias;
	BOOL	is_the_last = FALSE;

	if (h->f.is_short_option)
		is_the_last = !h->av[h->f.i][h->f.l + 1] || h->av[h->f.i][h->f.l + 1] == '=';

	h->f.found = FALSE;
	h->offset = 0;
	for /* each existing options */
		(h->f.n = 0 ;
		 !h->f.error && !h->f.found && h->f.n < i_hopt_c_maps ;
		 ++h->f.n)
	{
		h->oac = i_hopt_maps[h->f.n].argc;
		aliases = hopt_split(i_hopt_maps[h->f.n].names, '=');
		if (!aliases)
		{
			hopt_nerr = HOPT_MALLOCF;
			h->f.error = TRUE;
		}
		for /* each aliases of the current option */
			(h->f.m = 0 ;
			 !h->f.error && !h->f.found && aliases[h->f.m] ;
			 ++h->f.m)
		{
			h->f.last_i = h->f.i;
			alias = aliases[h->f.m];

			if (is_valid_option(alias, option, len))
			{
				if (h->oac == HOPT_VARIADIC_ARGUMENTS)
					h->oac = __oac_calcul_variadic_count(h, h->f.i, h->f.l);

				if (!i_hopt_flags[h->f.n] || (i_hopt_redef_allowed && i_hopt_redef_overwrt))
				{
					if (h->oac > 0 && h->f.is_short_option && !is_the_last)
						ERROR_SYSTEM(h, HOPT_BADSORDER, option, len);
					else
						ANHIHILATOR(h, option, len);
				}
				else if (i_hopt_redef_allowed)
				{
					if (!(h->f.is_short_option && is_the_last && h->oac == 1 && option[1] == '='))
					{
						h->f.i += h->oac;
						h->n_parsed += h->oac;
					}
				}
				h->f.found = TRUE;
			}
			free(aliases[h->f.m]);
		}
		for ( ; aliases[h->f.m] ; ++h->f.m)
			free(aliases[h->f.m]);
		free(aliases);

		if (!h->f.error && h->f.found)
			SLAVE(h, option, len);
	}
	// if (!h->f.error && !h->f.found && !i_hopt_undef_allowed)
	// 	FINDER_ERROR(h, HOPT_UNDEFINED, i, j);
	// else if (h->f.error == FALSE && h->av[i])
	// 	is_an_option = h->av[i][0] == '-' && strnlen(&h->av[i][0], 2) > 1;
}

BOOL
SEVEN_COMMANDS(const char* argument, unsigned int size)
{
	// Search the next subcommand
	for (unsigned int cmt = 1 ; cmt <= hopt_c_states ; ++cmt)
	{
		if (!strcmp(argument, hopt_state[cmt]._hopt_cmd_name))
		{
			char*			last_hierarchy = i_hopt_cmd_hierarchy;
			unsigned int	last_state = hopt_current_state;

			hopt_current_state = cmt;
			i_hopt_flags = calloc(i_hopt_c_maps, sizeof(BOOL));
			// There is a hierarchy
			if (last_hierarchy && i_hopt_cmd_hierarchy)
			{
				unsigned int	last_hierarchy_size = 0;
				unsigned int	last_cmd_name_size = strlen(i_hopt_cmd_name);

				if (last_hierarchy)
					last_hierarchy_size = strlen(last_hierarchy);
				// The parent root hierarchy is the same
				if (!strncmp(last_hierarchy, i_hopt_cmd_hierarchy, last_hierarchy_size))
				{
					// // Check if the next command in current hierarchy is the current command parsed
					unsigned int	current_hierarchy_size = strlen(i_hopt_cmd_hierarchy);
					if (last_hierarchy_size + 1 < current_hierarchy_size &&
						!strncmp(i_hopt_cmd_name, &i_hopt_cmd_hierarchy[last_hierarchy_size + 1], last_cmd_name_size))
					{
						__execute_subcommand_if_exists(last_state);
						return (TRUE);
					} // Worst code I wrote in my life
					else
					{
						hopt_current_state = last_state;
						continue ;
					}
				}
				else
				{
					hopt_current_state = last_state;
					continue ;
				}
			}
			else if (!last_hierarchy && i_hopt_cmd_hierarchy && strcmp(i_hopt_cmd_name, i_hopt_cmd_hierarchy))
			{
				hopt_current_state = last_state;
				continue ;
			}
			__execute_subcommand_if_exists(last_state);
			return (TRUE);
		}
	}
	return (FALSE);
}

void
BETTER_FINDER(t_hopt* hopt_restrict h)
{
	int			size;
	BOOL		is_an_option;
	BOOL		is_subcmd = FALSE;
	const char*	argument;

	h->f.i = 1;
	for ( ; !h->f.error && h->f.i < (unsigned int)h->ac && h->av[h->f.i] ; ++h->f.i) // Loop for each arguments
	{
		argument = h->av[h->f.i];
		is_an_option = argument[0] == '-' && argument[1];

		if (is_an_option)
		{
			if (argument[1] != '-')
				h->f.is_short_option = TRUE;
			else
				h->f.is_short_option = FALSE;

			if (!h->f.is_short_option)
			{
				size = strlen(&argument[1]);
				EXECUTOR(h, &argument[1], size);
				if (!h->f.error && !h->f.found && !i_hopt_undef_allowed)
					ERROR_SYSTEM(h, HOPT_UNDEFINED, &argument[1], size);
			}
			else
			{
				for (unsigned int l = 1 ; !h->f.error && argument[l] ; ++l)
				{
					h->f.l = l;
					if (argument[l + 1] == '=')
						size = strlen(&argument[l]);
					else
						size = 1;
					
					EXECUTOR(h, &argument[l], size);
					if (!h->f.error && !h->f.found && !i_hopt_undef_allowed)
						ERROR_SYSTEM(h, HOPT_UNDEFINED, &argument[l], size);
					
					if (size > 1)
						break ;
				}
			}
			++h->n_parsed;
		}
		else if (hopt_current_state <= hopt_c_states)
			is_subcmd = SEVEN_COMMANDS(argument, strlen(argument));

		if (!is_an_option && !is_subcmd && i_hopt_end_on_arg_v)
			break ;
	}
	__execute_subcommand_if_exists(hopt_current_state);
}

// Trizomic function
// void
// FINDER(t_hopt* hopt_restrict h)
// {
// 	unsigned int	i;
// 	int				j;
// 	unsigned int	n;
// 	unsigned int	m;

// 	i = 1;
// 	while (h->f.error == FALSE && i < (unsigned int)h->ac)
// 	{
// 		int avi_size = strlen(&h->av[i][1]);
// 		BOOL is_an_option = h->av[i][0] == '-' && strnlen(&h->av[i][0], 2) > 1;
// 		if (is_an_option)
// 		{
// 			h->f.is_short_option = FALSE;
// 			if (h->av[i][1] != '-' && strnlen(&h->av[i][1], 2) > 1)
// 				h->f.is_short_option = TRUE;
// 			j = 1;
// 			while (h->f.error == FALSE && h->av[i] && (h->av[i][j] && h->av[i][j] != '=') && is_an_option)
// 			{
// 				n = 0;
// 				h->f.found = FALSE;
// 				h->offset = 0;
// 				while (h->f.error == FALSE && n < i_hopt_c_maps && h->f.found == FALSE)
// 				{
// 					h->oac = i_hopt_maps[n].argc;
// 					char** alias = hopt_split(i_hopt_maps[n].names, '=');//FINDER_ALIAS(h, n);
// 					if (!alias)
// 					{
// 						hopt_nerr = HOPT_MALLOCF;
// 						h->f.error = TRUE;
// 						break ;
// 					}
// 					m = 0;
// 					while (h->f.error == FALSE && alias[m])
// 					{
// 						if (h->f.found == FALSE && h->f.error == FALSE)
// 						{
// 							h->f.last_i = i;
// 							if (h->oac == HOPT_VARIADIC_ARGUMENTS)
// 							{
// 								int	variadic_oac = __oac_calcul_variadic_count(h, i, j);
// 								h->oac = variadic_oac;
// 							}
// 							if (h->f.is_short_option == FALSE && is_valide_long_option(&h->av[i][1], alias[m]))
// 							{
// 								int	tmp_i = i;
// 								if (i_hopt_flags[n] == FALSE || itCanBeOverwritable(n))
// 								{
// 									tmp_i = FINDER_WRITE(h, i, 1, n);
// 									if (tmp_i == -1)
// 										FINDER_ERROR(h, HOPT_CBERROR, i, 1);
// 									i = (unsigned int)tmp_i;
// 								}
// 								if (i_hopt_flags[n] == FALSE)
// 									++h->n_parsed;
// 								h->f.found = TRUE;
// 								j = avi_size;
// 							}
// 							else if (h->f.is_short_option == TRUE && h->av[i][j] == alias[m][0])
// 							{
// 								if (h->oac > 0 && (h->av[i][j + 1] != '\0' && (h->av[i][j + 1] != '=' && h->oac == 1)))
// 									FINDER_ERROR(h, HOPT_BADSORDER, i, j);
// 								if (i_hopt_flags[n] == FALSE || itCanBeRedefOvwr(n))
// 									if (FINDER_WRITE(h, i, j, n) == -1)
// 										FINDER_ERROR(h, HOPT_CBERROR, i, j);
// 								if (i_hopt_flags[n] == FALSE && j == 1)
// 									++h->n_parsed;
// 								if (h->av[i][j + 1] == '\0')// || (h->av[i][j + 1] == '=' && h->oac == 1))
// 									i += h->oac;
// 								h->f.found = TRUE;
// 							}
// 						}
// 						free(alias[m]);
// 						++m;
// 					}
// 					if (i_hopt_flags[n] == TRUE && h->f.found == TRUE && !itCanBeRedefined(n))
// 						FINDER_ERROR(h, HOPT_REDEFINED, h->f.last_i, j);
// 					if (h->f.found == TRUE && h->f.error == FALSE && (i_hopt_flags[n] == FALSE || (itCanBeOverwritable(n) && i_hopt_flags[n] == TRUE)))
// 					{
// 						if (hopt_g_disable_sort == FALSE)
// 							hopt_add_back(&h->f.head, hopt_new_node(h->f.last_i, h->oac));
// 						i_hopt_flags[n] = TRUE;
// 					}
// 					free(alias);
// 					++n;
// 				}
// 				if (h->f.found == FALSE && h->f.error == FALSE && i_hopt_undef_allowed == FALSE)
// 					FINDER_ERROR(h, HOPT_UNDEFINED, i, j);
// 				else if (h->f.error == FALSE && h->av[i])
// 					is_an_option = h->av[i][0] == '-' && strnlen(&h->av[i][0], 2) > 1;
// 				++j;
// 			}
// 		}
// 		else if (i_hopt_end_on_arg_v == TRUE && hopt_current_state == hopt_c_states)
// 			break ;
// 		else if (hopt_current_state <= hopt_c_states)
// 		{
// 			// Search the next subcommand
// 			for (unsigned int cmt = 1 ; cmt <= hopt_c_states ; ++cmt)
// 			{
// 				if (!strcmp(h->av[i], hopt_state[cmt]._hopt_cmd_name))
// 				{
// 					char*			last_hierarchy = i_hopt_cmd_hierarchy;
// 					unsigned int	last_state = hopt_current_state;

// 					hopt_current_state = cmt;
// 					i_hopt_flags = calloc(i_hopt_c_maps, sizeof(BOOL));
// 					// There is a hierarchy
// 					if (last_hierarchy && i_hopt_cmd_hierarchy)
// 					{
// 						unsigned int	last_hierarchy_size = 0;
// 						unsigned int	last_cmd_name_size = strlen(i_hopt_cmd_name);

// 						if (last_hierarchy)
// 							last_hierarchy_size = strlen(last_hierarchy);
// 						// The parent root hierarchy is the same
// 						if (!strncmp(last_hierarchy, i_hopt_cmd_hierarchy, last_hierarchy_size))
// 						{
// 							// // Check if the next command in current hierarchy is the current command parsed
// 							unsigned int	current_hierarchy_size = strlen(i_hopt_cmd_hierarchy);
// 							if (last_hierarchy_size + 1 < current_hierarchy_size &&
// 								!strncmp(i_hopt_cmd_name, &i_hopt_cmd_hierarchy[last_hierarchy_size + 1], last_cmd_name_size))
// 							{
// 								__execute_subcommand_if_exists(last_state);
// 								break ;
// 							} // Worst code I wrote in my life
// 							else
// 							{
// 								hopt_current_state = last_state;
// 								continue ;
// 							}
// 						}
// 						else
// 						{
// 							hopt_current_state = last_state;
// 							continue ;
// 						}
// 					}
// 					else if (!last_hierarchy && i_hopt_cmd_hierarchy && strcmp(i_hopt_cmd_name, i_hopt_cmd_hierarchy))
// 					{
// 						hopt_current_state = last_state;
// 						continue ;
// 					}
// 					__execute_subcommand_if_exists(last_state);
// 					break;
// 				}
// 			}
// 		}
// 		++i;
// 	}
// 	__execute_subcommand_if_exists(hopt_current_state);
// }

inline
int
__oac_calcul_variadic_count(t_hopt* hopt_restrict h, unsigned int /*av index */ idx, unsigned int /*av[X] index*/ c)
{
	int		i = 0;
	int		index = idx;
	BOOL	is_an_option = FALSE;

	if (h->f.is_short_option && (h->av[index][c + 1] != '\0' && (h->av[index][c + 1] != '=' && h->oac == 1)))
		return (0);
	++index;
	for ( ; i < __INT_MAX__ && h->av[index] ; )
	{
		is_an_option = h->av[index][0] == '-' && strnlen(&h->av[index][0], 2) > 1;
		if (is_an_option || !h->av[index])
			break ;
		++index;
		++i;
	}
	return (i);
}

BOOL
__check_any_hierarchy_coherence(char** splitted_hierarchy, unsigned int index)
{
	char**	cmds = splitted_hierarchy;
	char**	i_cmds = hopt_split(hopt_state[index]._hopt_cmd_hierarchy, HOPT_SSS_CHAR);

	if (!cmds)
	{
		if (i_cmds[0] && !i_cmds[1])
			return (TRUE);
		return (FALSE);
	}
	for (unsigned int i = 0 ; i_cmds[i] && cmds[i] ; ++i)
	{
		if (!strcmp(i_cmds[i], cmds[i]))
		{
			if (!cmds[i + 1] && i_cmds[i + 1] && !i_cmds[i + 2])
			{
				return (TRUE);
			}
			//else
			//{ add a boolean flag to avoid strcmp if not found, but continue the loop to free}
		}
		free(i_cmds[i]);
	}
	free(i_cmds);
	return (FALSE);
}

void
__hopt_find_missing_mandatory(t_hopt* hopt_restrict h)
{
	int				f = FALSE;
	char**			s;
	unsigned int	size;

	for (unsigned int k = 0 ; k <= hopt_c_states ; ++k)
	{
		t_hopt_state*	tmp = &hopt_state[hopt_c_states];
		for (unsigned int i = 0, j = 0 ; i < tmp->_hopt_c_maps ; ++i)
		{
			if (tmp->_hopt_maps[i].mandatory == TRUE)
			{
				if (f == TRUE)
				{
					hopt_nerr = HOPT_MISSOPT;
					s = hopt_split(tmp->_hopt_maps[i].names, '=');
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
__hopt_cathm_group(t_hopt_state* hopt_restrict state, char* category_name, unsigned int lenmax)
{
	int		buffersize = lenmax + strlen(category_name) + 10;
	char*	line = malloc((buffersize + 1) * sizeof(char));
	line[buffersize] = '\0';
	snprintf(line, buffersize, "\n\e[1m  %-*s\e[0m\n", (int)lenmax, category_name);
	state->_hopt_help_menu_str = hopt_strfjoin(state->_hopt_help_menu_str, line);
	free(line);
}

unsigned int
__hopt_calcul_lenmax_for_help_menu(t_hopt_state* hopt_restrict state)
{
	unsigned int	lenmax = 0;

	for (unsigned int i = 0 ; i < state->_hopt_c_maps ; ++i)
	{
		if (state->_hopt_maps[i].desc != NULL)
		{
			char**	splitted = hopt_split(state->_hopt_maps[i].names, '=');
			char*	aliases = hopt_strjoin("-", splitted[0]);
			free(splitted[0]);
			for (int j = 1 ; splitted[j] ; ++j)
			{
				aliases = hopt_strfvajoin(3, aliases, ", -", splitted[j]);
				free(splitted[j]);
			}
			free(splitted);
			if (strlen(aliases) > lenmax)
				lenmax = strlen(aliases);
			free(aliases);
		}
	}
	if (hopt_c_states > 0 && !state->_hopt_cmd_name)
	{
		lenmax += strlen("SUB-COMMANDS:");
		for (unsigned int i = 1 ; i <= hopt_c_states ; ++i)
			lenmax += strlen(hopt_state[i]._hopt_cmd_name);
	}
	return (lenmax);
}

int
__hopt_first_option_present(t_hopt_state* hopt_restrict state)
{
	for (unsigned int i = 0 ; i < state->_hopt_c_maps ; ++i)
		if (state->_hopt_maps[i].desc)
			return (i);
	return (0);
}

void
__hopt_generate_help_menu(t_hopt_state* hopt_restrict state)
{
	char*	endonarg_str = "";
	int		index = __hopt_first_option_present(state);

	if (state->_hopt_end_on_arg_v == FALSE)
		endonarg_str = "[OPTIONS...]";
	state->_hopt_help_menu_str =
		hopt_strvajoin(11,
			"Usage: ",
			basename(hopt_program_path),
			state->_hopt_cmd_name ? " " : "",
			state->_hopt_cmd_name ? state->_hopt_cmd_name : "",
			" [OPTIONS...] ",
			"ARGS... ",
			endonarg_str,
			"\n",
			state->_hopt_program_desc == NULL ? "" : state->_hopt_program_desc,
			state->_hopt_program_desc == NULL ? "" : "\n",
			state->_hopt_maps[index].group == NULL ? "\n" : "");

	unsigned int	lenmax = __hopt_calcul_lenmax_for_help_menu(state);
	char*			last_group = NULL;
	unsigned int	buffersize;
	char*			line;

	for (unsigned int i = 0 ; i < state->_hopt_c_maps ; ++i)
	{
		if (state->_hopt_maps[i].group && (!last_group || strcmp(last_group, state->_hopt_maps[i].group)))
		{
			last_group = state->_hopt_maps[i].group;
			__hopt_cathm_group(state, state->_hopt_maps[i].group, lenmax);
		}
		if (state->_hopt_maps[i].desc != NULL)
		{
			char**	aliases = hopt_split(state->_hopt_maps[i].names, '=');
			char*	alias = hopt_strjoin("-", aliases[0]);
			free(aliases[0]);
			for (int j = 1 ; aliases[j] ; ++j)
			{
				alias = hopt_strfvajoin(3, alias, ", -", aliases[j]);
				free(aliases[j]);
			}
			free(aliases);
			int		buffersize = lenmax + strlen(state->_hopt_maps[i].desc) + 8;
			char*	line = malloc((buffersize + 1) * sizeof(char));
			snprintf(line, buffersize, "  %-*s\t%s\n", (int)lenmax, alias, state->_hopt_maps[i].desc);
			line[buffersize] = '\0';
			state->_hopt_help_menu_str = hopt_strfjoin(state->_hopt_help_menu_str, line);
			free(alias);
			free(line);
		}
	}
	if (hopt_c_states > 0)
	{
		char**	splitted_hierarchy = hopt_split(state->_hopt_cmd_hierarchy, HOPT_SSS_CHAR);
		for (unsigned int i = 1 ; i <= hopt_c_states ; ++i)
		{
			BOOL	coherence;

			coherence = __check_any_hierarchy_coherence(splitted_hierarchy, i);
			if (coherence)
			{
				unsigned int	desc_size = hopt_state[i]._hopt_program_desc ? strlen(hopt_state[i]._hopt_program_desc) : 0;
				buffersize = lenmax + desc_size + 8;
				line = malloc((buffersize + 1) * sizeof(char));
				snprintf(line, buffersize, "  %-*s\t%s\n", (int)lenmax, hopt_state[i]._hopt_cmd_name, hopt_state[i]._hopt_program_desc ? hopt_state[i]._hopt_program_desc : "");
				line[buffersize] = '\0';
				state->_hopt_help_menu_str = hopt_strfjoin(state->_hopt_help_menu_str, line);
				free(line);
			}
		}
		if (splitted_hierarchy)
			for (unsigned int i = 0 ; splitted_hierarchy[i] ; ++i)
				free(splitted_hierarchy[i]);
		free(splitted_hierarchy);
	}
}
