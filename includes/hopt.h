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

#ifndef HOPT_H
#define HOPT_H 1

	/**********************/
	/*                    */
	/*    INFORMATIONS    */
	/*                    */
	/**********************/

# define HOPT_VERSION	100

# ifndef HOPT_MAX_OPTIONS
// modifiable
#  define HOPT_MAX_OPTIONS	64
# endif

	/*********************/
	/*                   */
	/*    ERROR CODES    */
	/*                   */
	/*********************/

# define HOPT_SUCCESS		0	// Success, no error
# define HOPT_MALLOCF		1	// A malloc failed
# define HOPT_UNDEFINED		2	// An option is undefined
# define HOPT_REDEFINED		3	// An option is redefined
# define HOPT_BADSORDER		4	// A short option (who need args) in a string is in bad order
# define HOPT_MISSOARGC		5	// Option argument missing
# define HOPT_CBERROR		6	// When a callback function return -1 (essentially to stop parsing)
# define HOPT_MISSOPT		7	// If a mandatory option is not present
# define HOPT_BADTYPE_NUM	8	// Bad option argument type (not numeric)

	/********************/
	/*                  */
	/*    HOPT FLAGS    */
	/*                  */
	/********************/

# define _HOPT_FLAGS_BEGIN	_HOPT_TYPE_BEGIN

	/*  TYPE FLAGS  */
# define _HOPT_TYPE_BEGIN	HOPT_TYPE_DEFAULT
# define HOPT_TYPE_DEFAULT	0	// Set option type (incompatible with HOPT_FLCB)
# define HOPT_TYPE_STR		0	// Set option type (incompatible with HOPT_FLCB)
	/*  NUM TYPE FLAGS  */
# define _HOPT_TYPE_NUMBGIN	HOPT_TYPE_CHAR
# define HOPT_TYPE_CHAR		1	// Set option type (incompatible with HOPT_FLCB)
# define HOPT_TYPE_SHORT	2	// Set option type (incompatible with HOPT_FLCB)
# define HOPT_TYPE_INT		3	// Set option type (incompatible with HOPT_FLCB)
# define HOPT_TYPE_LONG		4	// Set option type (incompatible with HOPT_FLCB)
# define HOPT_TYPE_FLOAT	5	// Set option type (incompatible with HOPT_FLCB)
# define HOPT_TYPE_DOUBLE	6	// Set option type (incompatible with HOPT_FLCB)
# define _HOPT_TYPE_NUMLAST	HOPT_TYPE_DOUBLE
# define _HOPT_TYPE_LAST	_HOPT_TYPE_NUMLAST

# define HOPT_FLCB			7	// Set callback (incompatible with HOPT_TYPE_*)

# define HOPT_FLMA			16	// Set an option as mandatory
# define HOPT_FLAGS_LAST	HOPT_FLMA

	/*******************/
	/*                 */
	/*     TYPEDEF     */
	/*                 */
	/*******************/

typedef int (*t_hopt_callback)(int, char**, void*); //ac, av, cb_arg

typedef struct hopt_map
{
	char			mandatory;
	char*			desc;
	char*			names;
	int				argc;
	void*			mem;
	int				flag;
	t_hopt_callback	cb;
	void*			cb_arg;
}	t_hopt_map;

//void hopt_early_exit(BOOL (*fun)(char* option, int optlen));

// Error code to specify error (look at 'hopt_cerr' to see which option is causing the error)
extern int		hopt_nerr;
// The option that refers to the error stored in 'hopt_nerr'
extern char		hopt_cerr[16];

	/*
		CORE for HOPT
	*/

// Parse and interpret options for you :0
// Call HOPT_ADD_OPTION(...) for each option before
// AV will be sorted after
//
// @param ac Arguments' program count
// @param av Total arguments's program
int		hopt(int ac, char** av);

void	hopt_free(void);

// Create new option mapping
//
// @param name Option aliases (ex: "c=-count=j"), option '-c' has 2 others alises : '--count' and '-j'
// @param argc Count of option's arguments
// @param flag HOPT flag
// @param ... Type of each option's argument(s)
//
// - if (flag == HOPT_TYPE_*):	Address in memory to fill (void*)
//
// - if (flag == HOPT_FLCB):	A callback function and his argument (t_hopt_callback, void*)
int		hopt_add_option(char* names, int argc, int flag, ...);

	/*
		UTILS for HOPT
	*/

// Return a string describe error (returned str must be free'd)
char*	hopt_strerror(void);

	/*
		OPTIONS for HOPT
	*/

// Set program description
void	hopt_program_description(char* program_desc);
// Undef allowed is not sorted, same for undef unallowed
void	hopt_allow_undef(void);
// @param overwrite The redefinition will overwrite the ancient definition (1), OR just be ignored (0) ?
void	hopt_allow_redef(char overwrite);
// Stop parsing on first non-option argument
void	hopt_end_on_arg(void);
// Automatically show help menu if there is any undefined option
//
// @param enable_256termcolor Active bold and colored text (not working yet)
void	hopt_auto_help(char enable_256termcolor);
// Get help menu
char*	hopt_help_menu(void);
void	hopt_disable_sort(void);
void	hopt_reset(void);

#endif
