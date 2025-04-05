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

#ifndef __HOPT__H
#define __HOPT__H

# define _XOPEN_SOURCE 700
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <stdarg.h>
# include "hopt.h"

# define BOOL	char
# define TRUE	1
# define FALSE	0

//# define CASSERT(condition) ((void)sizeof(char[-!(condition)]))

# define hopt_end_on_arg_v		hopt_state._hopt_end_on_arg_v
# define hopt_undef_allowed		hopt_state._hopt_undef_allowed
# define hopt_redef_allowed		hopt_state._hopt_redef_allowed
# define hopt_redef_overwrt		hopt_state._hopt_redef_overwrt
# define hopt_disable_sort_v	hopt_state._hopt_disable_sort_v
# define hopt_mallocd			hopt_state._hopt_mallocd
# define hopt_c_mallocd			hopt_state._hopt_c_mallocd
# define hopt_maps				hopt_state._hopt_maps
# define hopt_c_maps			hopt_state._hopt_c_maps

typedef struct hopt_sort
{
	unsigned int		index;
	unsigned int		argc;
	struct hopt_sort*	next;
}	t_hopt_sort;

typedef struct FINDER
{
	BOOL			error;
	BOOL			found;
	BOOL			strso;		// Know if is a STRing of Shorts Options
	unsigned int	last_i;		// Last index, to know the begin position of an option with argument(s)
	t_hopt_sort*	head;
	t_hopt_sort*	last_node;
	unsigned int	addrs_idx;
}	t_FINDER;

typedef struct hopt_state
{
	//BOOL (*hopt_cb_earlyquit)(const char*, unsigned int) = NULL;
	BOOL			_hopt_end_on_arg_v;
	BOOL			_hopt_undef_allowed;
	BOOL			_hopt_redef_allowed;
	BOOL			_hopt_redef_overwrt;
	BOOL			_hopt_disable_sort_v;
	void**			_hopt_mallocd;
	unsigned int	_hopt_c_mallocd;
	t_hopt_map*		_hopt_maps[HOPT_MAX_OPTIONS];
	unsigned int	_hopt_c_maps;
	//char*			_hopt_help_str		= NULL;	// !WORK IN PROGRESS!
}	t_hopt_state;

typedef struct hopt
{
	int				ac;			// Program arguments count
	char**			av;			// Program arguments value
	BOOL*			flags;		// To check if an option is redefined
	unsigned int	oac;		// Option Argument Count
	unsigned int	offset;		// Offset in the buffer
	unsigned int	n_parsed;	// Count of element parsed (included options' arguments)
	t_FINDER		f;			// FINDER(...) infos
}	t_hopt;

extern t_hopt_state	hopt_state;

// test.c
void			hopt_test_alignment(void);

// lst.c
t_hopt_sort*	hopt_new_node(unsigned int index, unsigned int argc);
void			hopt_add_front(t_hopt_sort** head, t_hopt_sort* node);
void			hopt_add_back(t_hopt_sort** head, t_hopt_sort* node);
void			hopt_free_lstsort(t_hopt_sort* head);

// utils.c
char**			strsplit(const char* restrict s, char sep);
void			free2(void**  ptr2);
unsigned int	strlen2(char* restrict* restrict s);

// core.c
int
SORT(int ac, /*const*/ char** av, t_hopt_sort* head);
void
FINDER(t_hopt* restrict h);

#endif
