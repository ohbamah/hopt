# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bama <bama@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/13 16:21:19 by ymanchon          #+#    #+#              #
#    Updated: 2025/04/04 22:09:51 by bama             ###   ########.fr        #
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

NAME = libhopt.a
AR = @ar -rcs

CC = @cc -std=c18

LMAKE = @make --no-print-directory -C

SRCS =	./srcs/hopt.c \
		./srcs/lst.c \
		./srcs/utils.c \
		./srcs/core.c

OBJS_DIR = .objs

OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.obj)

DEPS = $(OBJS:%.obj=%.d)

CFLAGS = -Wall -Wextra -O3 -fPIE -MMD -Wno-stringop-truncation -g3

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
