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

int				hopt_nerr = HOPT_SUCCESS;		// extern global var in 'hopt.h'
char			hopt_cerr[HOPT_MAX_SSTR_SIZE] = {0};// extern global var in 'hopt.h'

int				hopt_fcmd = HOPT_NOCMD;
char			hopt_ncmd[HOPT_MAX_SSTR_SIZE] = {0};
char			hopt_ccmd[HOPT_MAX_SSTR_SIZE] = {0};
char			hopt_pcmd[HOPT_MAX_SSTR_SIZE] = {0};

char**				hopt_flags = NULL;
t_hopt_global_state	hopt_global_state = {0, .fd = 1};
t_hopt_state		hopt_default_state = {0};
t_hopt_state*		hopt_state = &hopt_default_state;
unsigned int		hopt_c_states = 0;
unsigned int		hopt_current_state = 0;

void
__hopt_generate_help_menu(t_hopt_state* hopt_restrict);

// Parse and interpret options for you :0
// Call HOPT_ADD_OPTION(...) for each option before
//
// @param ac Arguments' program count
// @param av Total arguments's program
int
hopt(int ac, char** av)
{
	t_hopt			h = {0};

	hopt_program_path = av[0];
	h.ac = ac;
	h.av = av;
	hopt_flags = calloc(hopt_c_states + 1, sizeof(BOOL*));
	hopt_flags[0] = calloc(hopt_state[0]._hopt_c_maps, sizeof(BOOL));
	FINDER(&h);
	if (h.f.mandatory_count != hopt_c_mandatory)
	{
		__hopt_find_missing_mandatory(&h);
		hopt_free_lstsort(h.f.head);
	}
	else if (h.f.error == FALSE && hopt_g_disable_sort == FALSE)
		SORT(ac, av, h.f.head);
	else if (hopt_g_disable_sort == FALSE)
		hopt_free_lstsort(h.f.head);
	if (hopt_nerr != HOPT_SUCCESS)
		return (-1);
	return (h.n_parsed);
}

static inline
void
hopt_map_check_flags(t_hopt_map* map, int flags, va_list* va)
{
	int	type = flags & 0xF;
	if (type >= _HOPT_TYPE_BEGIN && type <= _HOPT_TYPE_LAST)
		(*map).mem = va_arg(*va, void*);
	else if (type == HOPT_FLCB)
	{
		(*map).mem = NULL;
		(*map).cb = va_arg(*va, t_hopt_callback);
		(*map).cb_arg = va_arg(*va, void*);
	}
	if (flags & HOPT_FLMA)
	{
		(*map).mandatory = TRUE;
		++hopt_c_mandatory;
	}
	else
		(*map).mandatory = FALSE;
}

static
t_hopt_map
hopt_create_map(char* names, int argc, int flags, va_list* va)
{
	t_hopt_map	map;
	map.names = names;
	map.argc = argc;
	map.flag = flags;
	hopt_map_check_flags(&map, flags, va);
	map.desc = va_arg(*va, char*);
	map.group = hopt_group_title;
	return (map);
}

// Create new option mapping
//
// @param name Option aliases (ex: "c=-count=j"), option '-c' has 2 others alises : '--count' and '-j'
// @param argc Count of option's arguments
// @param mem Address in memory to fill
// @param ... Type of each option's argument(s)
int
hopt_add_option(char* names, int argc, int flag, ...)
{
	va_list	va;
	va_start(va, flag);
	hopt_maps = realloc(hopt_maps, (hopt_c_maps + 1) * sizeof(t_hopt_map));
	memset(&hopt_maps[hopt_c_maps], 0, sizeof(t_hopt_map));
	hopt_maps[hopt_c_maps] = hopt_create_map(names, argc, flag, &va);
	++hopt_c_maps;
	va_end(va);
	return (0);
}

void
hopt_free(void)
{
	for (unsigned int i = 0 ; i <= hopt_c_states ; ++i)
	{
		free(hopt_state[i]._hopt_help_menu_str);
		free(hopt_state[i]._hopt_maps);
		free(hopt_flags[i]);
	}
	if (hopt_state != &hopt_default_state)
		free(hopt_state);
	free(hopt_flags);
}

// Return a string describe error (returned str must be free'd)
char*
hopt_strerror(void)
{
	char*	test;
	int		size = strlen(hopt_cerr);

	switch (hopt_nerr)
	{
		case HOPT_MALLOCF:
			test = malloc((21 + 1) * sizeof(char));
			strncpy(test, "hopt: malloc failed.", 21);
			test[21] = 0;
			return (test);
		case HOPT_UNDEFINED:
			test = malloc((29 + size + 1) * sizeof(char));
			snprintf(test, 29 + size, "hopt: option -%s is undefined.", hopt_cerr);
			test[29 + size] = 0;
			return (test);
		case HOPT_REDEFINED:
			test = malloc((29 + size + 1) * sizeof(char));
			snprintf(test, 29 + size, "hopt: option -%s is redefined.", hopt_cerr);
			test[29 + size] = 0;
			return (test);
		case HOPT_BADSORDER:
			test = malloc((111 + size + 1) * sizeof(char));
			snprintf(test, 111 + size, "hopt: option -%s is in bad order, he need argument(s) but is not in last position in a string of shorts options.", hopt_cerr);
			test[111 + size] = 0;
			return (test);
		case HOPT_MISSOARGC:
			test = malloc((33 + size + 1) * sizeof(char));
			snprintf(test, 33 + size, "hopt: option -%s miss argument(s).", hopt_cerr);
			test[33 + size] = 0;
			return (test);
		case HOPT_CBERROR:
			test = malloc((42 + size + 1) * sizeof(char));
			snprintf(test, 42 + size, "hopt: intern callback error with option %s.", hopt_cerr);
			test[42 + size] = 0;
			return (test);
		case HOPT_MISSOPT:
			test = malloc((36 + size + 1) * sizeof(char));
			snprintf(test, 36 + size, "hopt: mandatory option %s is missing.", hopt_cerr);
			test[36 + size] = 0;
			return (test);
		case HOPT_BADTYPE_NUM:
			test = malloc((49 + size + 1) * sizeof(char));
			snprintf(test, 49 + size, "hopt: option %s need strict numeric argument only.", hopt_cerr);
			test[49 + size] = 0;
			return (test);
		default:
			test = malloc((15 + 1) * sizeof(char));
			strncpy(test, "hopt: success.", 15);
			test[15] = 0;
			return (test);
	}
}

// @param overwrite The redefinition will overwrite the ancient definition (1), OR just be ignored (0) ?
void
hopt_allow_redef(BOOL overwrite)
{
	hopt_redef_overwrt = overwrite;
	hopt_redef_allowed = TRUE;
}

// Undef allowed is not sorted, same for undef unallowed
void
hopt_allow_undef(void)
{
	hopt_undef_allowed = TRUE;
}

// Stop parsing on first non-option argument
void
hopt_end_on_arg(void)
{
	hopt_end_on_arg_v = TRUE;
}

// No sort AV
void
hopt_disable_sort(void)
{
	hopt_g_disable_sort = TRUE;
}

void
hopt_program_description(char* program_desc)
{
	hopt_g_program_desc = program_desc;
}

void
hopt_group(char* group_name)
{
	hopt_group_title = group_name;
}

void
hopt_subcmd(char* cmd)
{
	++hopt_c_states;
	if (hopt_state == &hopt_default_state)
	{
		t_hopt_state*	tmp = &hopt_default_state;
		hopt_state = malloc(sizeof(t_hopt_state) * 2);
		memcpy(&hopt_state[0], tmp, sizeof(t_hopt_state));
	}
	else
		hopt_state = realloc(hopt_state, (hopt_c_states + 1) * sizeof(t_hopt_state));
	memset(&hopt_state[hopt_c_states], 0, sizeof(t_hopt_state));
	hopt_group_title = NULL;
	hopt_cmd_name = cmd;
}

char*
hopt_help_menu(char* cmd)
{
	if (cmd == NULL)
	{
		__hopt_generate_help_menu(&hopt_state[0]);
		return (hopt_state[0]._hopt_help_menu_str);
	}
	for (unsigned int i = 1 ; i <= hopt_c_states ; ++i)
	{
		t_hopt_state*	tmp = &hopt_state[i];
		if (!strcmp(cmd, tmp->_hopt_cmd_name))
		{
			if (tmp->_hopt_help_menu_str == NULL)
				__hopt_generate_help_menu(tmp);
			return (tmp->_hopt_help_menu_str);
		}
	}
	return (NULL);
}

void	hopt_set_fd(int fd)
{
	hopt_g_fd = fd;
}

void	hopt_set_file(FILE* file)
{
	hopt_g_file = file;
}

void	hopt_help_option(char* aliases, int automatic, int flagswhen)
{
	hopt_add_option(aliases, 0, HOPT_FLCB, (t_hopt_callback)__hopt_intern_print_help_menu, hopt_cmd_name, NULL);
	if (flagswhen == 0)
		hopt_help_flagsw = (HOPT_UNDEFINED | HOPT_REDEFINED | HOPT_MISSOARGC | HOPT_MISSOPT);
	else
		hopt_help_flagsw = flagswhen;
	hopt_auto_help_v = automatic;
}

inline
void	hopt_print_help_menu(char* cmd)
{
	if (hopt_g_file == NULL)
		dprintf(hopt_g_fd, "%s\n", hopt_help_menu(cmd));
	else
		fprintf(hopt_g_file, "%s\n", hopt_help_menu(cmd));
}

void
hopt_reset(void)
{
	memset(hopt_cerr, 0, 16);
	hopt_nerr = 0;
	hopt_undef_allowed = FALSE;
	hopt_redef_allowed = FALSE;
	hopt_redef_overwrt = FALSE;
	hopt_end_on_arg_v = FALSE;
}
