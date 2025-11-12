# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/13 16:21:19 by ymanchon          #+#    #+#              #
#    Updated: 2025/03/25 15:19:52 by ymanchon         ###   ########.fr        #
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

NAME = hopt_unit_tester

CC = @clang++

LMAKE = @make --no-print-directory -C

# ############## #
#*    REGLES    *#
# ############## #

all: compile

compile:
	$(CC) -g3 -fPIE unit_tester.cpp -Wno-unused-value -L.. -lhopt -o $(NAME)

# Tests unitaires avec votre système
unit:
	@echo -e "$(YELLOW)$(BOLD)Compilation des tests unitaires...$(CLASSIC)"
	@clang++ -fPIE unit_tester.cpp -Wno-unused-value -Wno-writable-strings -L.. -lhopt -o unit_tests
	@echo -e "$(YELLOW)$(BOLD)Exécution des tests unitaires...$(CLASSIC)"
	@./unit_tests
	@rm -f unit_tests

.PHONY: test unit
