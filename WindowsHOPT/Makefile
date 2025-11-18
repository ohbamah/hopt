#
 #* MIT License
 #* 
 #* Copyright (c) 2025 OhBama (Yann Manchon)
 #* 
 #* Permission is hereby granted, free of charge, to any person obtaining a copy
 #* of this software and associated documentation files (the "Software"), to deal
 #* in the Software without restriction, including without limitation the rights
 #* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 #* copies of the Software, and to permit persons to whom the Software is
 #* furnished to do so, subject to the following conditions:
 #* 
 #* The above copyright notice and this permission notice shall be included in all
 #* copies or substantial portions of the Software.
 #* 
 #* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 #* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 #* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 #* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 #* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 #* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 #* SOFTWARE.
#

# ################ #
#*    COULEURS    *#
# ################ #

CLASSIC	= \033[0m
WHITE	= \033[38;2;238;232;213m
BOLD	= \033[1m
# BLACK	= \033[38;2;0;0;0m
BLACK	= \033[30m
RED   	= \033[31m
GREEN  	= \033[32m
BLUE 	= \033[38;2;0;36;148m
PURPLE 	= \033[38;2;211;54;130m
CYAN   	= \033[36m
YELLOW 	= \033[38;2;250;189;0m

# ############### #
#*   VARIABLES   *#
# ############### #

NAME = libhopt.lib
AR = @llvm-lib

CC = @clang

LMAKE = @make --no-print-directory -C

SRCS =	../srcs/hopt.c \
		../srcs/lst.c \
		../srcs/utils.c \
		../srcs/core.c

OBJS_DIR = .objs

OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.obj)

DEPS = $(OBJS:%.obj=%.d)

CFLAGS = -Wall -Wextra -Wno-unused-result -MMD -g3 -Wno-unused-variable -Wno-unused-parameter -Wno-deprecated-declarations

INCLUDES = -I.. -I../includes/

SHELL = cmd

# ############## #
#*    REGLES    *#
# ############## #

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(OBJS) /OUT:$(NAME)
	@echo "Librairie $(NAME) créé avec succès!"

$(OBJS_DIR)/%.obj: %.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	@echo "Compilation $<"
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJS_DIR) -rf

fclean: clean
	rm $(LIBNAME) -f
	rm $(NAME) -f
	@echo "Tout a été supprimé... 🗑️\n"

re: fclean all

libft_comp:
	$(LMAKE) $(LIBFT_P)

.PHONY: all clean fclean re libft_comp
-include $(DEPS)
