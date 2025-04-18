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

# ifdef _MSC_VER
#  define hopt_restrict	__restrict
# elif defined(__GNUC__) || defined(__clang__)
#  define hopt_restrict	__restrict__
# elif defined(__INTEL_COMPILER)
#  define hopt_restrict	restrict
# else
#  define hopt_restrict
# endif

# if defined(__unix__) || defined(__APPLE__) || defined(__linux__)
#  define _HOPT_LIBGEN_COMPABILITY
#  include <libgen.h>
# endif

# define BOOL	char
# define TRUE	1
# define FALSE	0

//# define CASSERT(condition) ((void)sizeof(char[-!(condition)]))

# define hopt_end_on_arg_v		hopt_state[hopt_c_states]._hopt_end_on_arg_v
# define hopt_undef_allowed		hopt_state[hopt_c_states]._hopt_undef_allowed
# define hopt_redef_allowed		hopt_state[hopt_c_states]._hopt_redef_allowed
# define hopt_redef_overwrt		hopt_state[hopt_c_states]._hopt_redef_overwrt
# define hopt_auto_help_v		hopt_state[hopt_c_states]._hopt_auto_help_v
# define hopt_maps				hopt_state[hopt_c_states]._hopt_maps
# define hopt_c_maps			hopt_state[hopt_c_states]._hopt_c_maps
# define hopt_c_mandatory		hopt_state[hopt_c_states]._hopt_c_mandatory
# define hopt_help_menu_str		hopt_state[hopt_c_states]._hopt_help_menu_str
# define hopt_program_path		hopt_state[hopt_c_states]._hopt_program_path
# define hopt_cmd_name			hopt_state[hopt_c_states]._hopt_cmd_name
# define hopt_help_flagsw		hopt_state[hopt_c_states]._hopt_help_flagsw
# define hopt_group_title		hopt_state[hopt_c_states]._hopt_group_title

# define i_hopt_end_on_arg_v	hopt_state[hopt_current_state]._hopt_end_on_arg_v
# define i_hopt_undef_allowed	hopt_state[hopt_current_state]._hopt_undef_allowed
# define i_hopt_redef_allowed	hopt_state[hopt_current_state]._hopt_redef_allowed
# define i_hopt_redef_overwrt	hopt_state[hopt_current_state]._hopt_redef_overwrt
# define i_hopt_auto_help_v		hopt_state[hopt_current_state]._hopt_auto_help_v
# define i_hopt_maps			hopt_state[hopt_current_state]._hopt_maps
# define i_hopt_c_maps			hopt_state[hopt_current_state]._hopt_c_maps
# define i_hopt_c_mandatory		hopt_state[hopt_current_state]._hopt_c_mandatory
# define i_hopt_help_menu_str	hopt_state[hopt_current_state]._hopt_help_menu_str
# define i_hopt_program_path	hopt_state[hopt_current_state]._hopt_program_path
# define i_hopt_cmd_name		hopt_state[hopt_current_state]._hopt_cmd_name
# define i_hopt_help_flagsw		hopt_state[hopt_current_state]._hopt_help_flagsw
# define i_hopt_group_title		hopt_state[hopt_current_state]._hopt_group_title

# define i_hopt_flags			hopt_flags[hopt_current_state]

# define hopt_g_fd				hopt_global_state.fd
# define hopt_g_file			hopt_global_state.file
# define hopt_g_disable_sort	hopt_global_state.disable_sort_v
# define hopt_g_program_desc	hopt_global_state.program_desc

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
	unsigned int	addrs_idx;
	unsigned int	mandatory_count;	// Follow the count of mandatory option
}	t_FINDER;

typedef struct hopt
{
	int				ac;			// Program arguments count
	char**			av;			// Program arguments value
	unsigned int	oac;		// Option Argument Count
	unsigned int	offset;		// Offset in the buffer
	unsigned int	n_parsed;	// Count of element parsed (included options' arguments)
	t_FINDER		f;			// FINDER(...) infos
}	t_hopt;

typedef struct hopt_global_state
{
	BOOL	disable_sort_v;
	int		fd;
	FILE*	file;
	char*	program_desc;
}	t_hopt_global_state;

typedef struct hopt_state
{
/*bool*/char			_hopt_end_on_arg_v;
/*bool*/char			_hopt_undef_allowed;
/*bool*/char			_hopt_redef_allowed;
/*bool*/char			_hopt_redef_overwrt;
/*bool*/char			_hopt_auto_help_v;
/*bool*/char			_hopt_256termcolor_v;
		int				_hopt_help_flagsw;
		char*			_hopt_help_menu_str;
		char*			_hopt_program_path;
		char*			_hopt_group_title;
		char*			_hopt_cmd_name;
		t_hopt_map*		_hopt_maps;
		unsigned int	_hopt_c_maps;
		unsigned int	_hopt_c_mandatory;
}	t_hopt_state;

extern /*bool*/char**		hopt_flags; // To check if an option is redefined
extern t_hopt_state			hopt_default_state;
extern t_hopt_state*		hopt_state;

extern t_hopt_global_state	hopt_global_state;
extern unsigned int			hopt_c_states;
extern unsigned int			hopt_current_state;

// lst.c
t_hopt_sort*	hopt_new_node(unsigned int index, unsigned int argc);
void			hopt_add_front(t_hopt_sort** head, t_hopt_sort* node);
void			hopt_add_back(t_hopt_sort** head, t_hopt_sort* node);
void			hopt_free_lstsort(t_hopt_sort* head);

// utils.c
char**			strsplit(const char* hopt_restrict s, char sep);
void			free2(void**  ptr2);
unsigned int	strlen2(char* hopt_restrict* hopt_restrict s);
char*			hopt_strvajoin(unsigned int count, ...);
char*			hopt_strjoin(const char* hopt_restrict s1, const char* hopt_restrict s2);
# ifndef _HOPT_LIBGEN_COMPABILITY
char*			basename(const char* hopt_restrict path);
# endif

// core.c
int
SORT(int ac, /*const*/ char** av, t_hopt_sort* head);
void
FINDER(t_hopt* hopt_restrict h);
void
__hopt_generate_help_menu(t_hopt_state* hopt_restrict state);
void
__hopt_find_missing_mandatory(t_hopt* hopt_restrict h);
void
__hopt_intern_print_help_menu(int, char**, char* cmd);

#endif
