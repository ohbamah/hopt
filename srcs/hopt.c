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

int				hopt_nerr = 0;		// extern global var in 'hopt.h'
char			hopt_cerr[16] = {0};// extern global var in 'hopt.h'

t_hopt_state	hopt_state = {0};

// Parse and interpret options for you :0
// Call HOPT_ADD_OPTION(...) for each option before
//
// @param ac Arguments' program count
// @param av Total arguments's program
int
hopt(int ac, char** av)
{
	t_hopt			h;

	memset(&h, 0, sizeof(h));
	h.ac = ac;
	h.av = av;
	h.flags = calloc(hopt_c_maps, sizeof(BOOL));
	FINDER(&h);
	if (hopt_disable_sort_v == FALSE)
		SORT(ac, av, h.f.head);
	free(h.flags);
	if (hopt_nerr != HOPT_SUCCESS)
		return (-1);
	return (h.n_parsed);
}

static inline
t_hopt_map*
hopt_create_map(char* names, int argc, int flag, va_list va)
{
	t_hopt_map* map = malloc(sizeof(t_hopt_map));
	if (!map)
		return (NULL); //! fatal error
	map->names = names;
	map->argc = argc;
	map->flag = flag;
	if (flag >= HOPT_TYPE_DEFAULT && flag <= HOPT_TYPE_LAST)
	{
		map->mem = va_arg(va, void*);
	}
	else if (flag == HOPT_FLCB)
	{
		map->mem = NULL;
		map->cb = va_arg(va, t_hopt_callback);
		map->cb_arg = va_arg(va, void*);
	}
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
	hopt_maps[hopt_c_maps] = hopt_create_map(names, argc, flag, va);
	++hopt_c_maps;
	va_end(va);
	return (0);
}

void
hopt_free(void)
{
	for (unsigned int i = 0 ; i < hopt_c_maps ; ++i)
	{
		free(hopt_maps[i]);
		hopt_maps[i] = NULL;
	}
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
		case HOPT_INVALID:
			test = malloc((34 + size + 1) * sizeof(char));
			snprintf(test, 34 + size, "hopt: option -%s arg count invalid.", hopt_cerr);
			test[34 + size] = 0;
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
		case HOPT_BADOPTSTR:
			test = malloc((76 + size + 1) * sizeof(char));
			snprintf(test, 76 + size, "hopt: option -%s in bad string format, call hopt_help() to see string format.", hopt_cerr);
			test[76 + size] = 0;
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
	hopt_disable_sort_v = TRUE;
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
