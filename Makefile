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

CLASSIC	= \e[0m
WHITE	= \e[38;2;238;232;213m
BOLD	= \e[1m
# BLACK	= \e[38;2;0;0;0m
BLACK	= \e[30m
RED   	= \e[31m
GREEN  	= \e[32m
BLUE 	= \e[38;2;0;36;148m
PURPLE 	= \e[38;2;211;54;130m
CYAN   	= \e[36m
YELLOW 	= \e[38;2;250;189;0m

# ############### #
#*   VARIABLES   *#
# ############### #

NAME = libhopt.a
AR = @ar -rcs

CC = @clang

LMAKE = @make --no-print-directory -C

SRCS =	./srcs/hopt.c \
		./srcs/lst.c \
		./srcs/utils.c \
		./srcs/helper.c \
		./srcs/core.c

OBJS_DIR = .objs

OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.obj)

DEPS = $(OBJS:%.obj=%.d)

CFLAGS = -Wall -Wextra -O3 -fPIE -MMD -g3 #-Wno-stringop-truncation

INCLUDES = -I. -I./includes/


# ############## #
#*    REGLES    *#
# ############## #

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)
	@echo "$(BOLD)$(CYAN)Librairie $(NAME) créé avec succès!$(CLASSIC)"

$(OBJS_DIR)/%.obj: %.c
	@mkdir -p $(@D)
	@echo "$(GREEN)🗸 Compilation $(BOLD)$(YELLOW)$<$(CLASSIC)"
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(BOLD)$(RED)"
	rm -rf $(OBJS_DIR)
	@echo "$(CLASSIC)"

fclean: clean
	@echo "$(BOLD)$(RED)"
	rm -f $(LIBNAME)
	rm -f $(NAME)
	@echo "$(BOLD)$(GREEN)Tout a été supprimé... 🗑️\n$(CLASSIC)"

re: fclean all

libft_comp:
	$(LMAKE) $(LIBFT_P)

.PHONY: all clean fclean re libft_comp
-include $(DEPS)
