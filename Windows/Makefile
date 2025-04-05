# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bama <bama@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/13 16:21:19 by ymanchon          #+#    #+#              #
#    Updated: 2025/04/05 13:31:40 by bama             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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

NAME = hopt.lib
AR = @llvm-lib

CC = @clang

LMAKE = @make --no-print-directory -C

SRCS =	../srcs/hopt.c \
		../srcs/lst.c \
		../srcs/utils.c \
		../srcs/core.c

OBJS_DIR = ../.objs

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
	@echo "$(BOLD)$(CYAN)Librairie $(NAME) créé avec succès!$(CLASSIC)"

$(OBJS_DIR)/%.obj: %.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	@echo "$(GREEN)🗸 Compilation $(BOLD)$(YELLOW)$<$(CLASSIC)"
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(BOLD)$(RED)"
	rm $(OBJS_DIR) -rf
	@echo "$(CLASSIC)"

fclean: clean
	@echo "$(BOLD)$(RED)"
	rm $(LIBNAME) -f
	rm $(NAME) -f
	@echo "$(BOLD)$(GREEN)Tout a été supprimé... 🗑️\n$(CLASSIC)"

re: fclean all

libft_comp:
	$(LMAKE) $(LIBFT_P)

.PHONY: all clean fclean re libft_comp
-include $(DEPS)
