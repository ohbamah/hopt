#!/bin/bash

CLASSIC="\e[0m"
WHITE="\e[38;2;238;232;213m"
BOLD="\e[1m"
#BLACK=\e[38;2;0;0;0m
BLACK="\e[30m"
RED="\e[31m"
GREEN="\e[32m"
BLUE="\e[38;2;0;36;148m"
PURPLE="\e[38;2;211;54;130m"
CYAN="\e[36m"
YELLOW="\e[38;2;250;189;0m"

TESTS=(
	"./hopt_tests -fp 2 1 4 5 6 3 --count 2 random_arg1 -c 3 random_arg2 --name "test_name" -test "str1" "str2" "str3""
	"./hopt_tests --count 10 -fp 2 4 --name "example" --pid 1 3 5 -test "a" "b""
	"./hopt_tests -f -p 9 8 7 6 --test "only_one" --count 5"
	"./hopt_tests --pid 1 2 3 4 5 -name "name1" -name "name2" -c 99"
	"./hopt_tests -test "one" "two" --name "hello" -p 2 4 6 8 10 12 -f --randomopt"
	"./hopt_tests -f -c 3 -p 3 1 --count 7 -name "X" --test "str" --test "strX" --test "strY""
	"./hopt_tests --count 5 -c 3 -f -test "first" "second" --pid 5 4 3 2 1 --name "XYZ""
	"./hopt_tests -p 6 7 8 -f --count --test "no_value""
	"./hopt_tests -p 3 1 -test "A" --name "B" -c 4 -f --test "C" "D""
	"./hopt_tests -test "Z" "X" -c -f -pid 3 2 --name --randomarg1 randomarg2"
	"./hopt_tests --name --pid 1 2 3 4 5 6 --count -test "one" --p 9 8"
	"./hopt_tests -f --test "wrong" -pid -count 4 -name "fail_test""
	"./hopt_tests -test "A" "B" -test "C" --count --pid 5 6 7 --unknownopt"
	"./hopt_tests -c 3 -f --test "arg" -pid 2 3 4 --test --name"
	"./hopt_tests -name -f -p 1 2 3 4 -test "ok" "ok2" --count --pid 6 7"
	"./hopt_tests --count 5 -- -p 1 2 3 4 5 -test "B" "C""
	"./hopt_tests -f -p 2 -c 3 -test "empty" --count 7 --name """
	"./hopt_tests -f -name "Test" -test --count 3 -pid 4 5 6"
	"./hopt_tests --test -count 5 -name -pid 1 2 3 4 5 6 -f"
	"./hopt_tests -f -p 1 2 3 4 5 --count 3 -name "OptionTest" -test "One" "Two""
	"./hopt_tests -count -p 1 2 3 4 5 6 -test "First" "Second" "Third""
)

for i in "${!TESTS[@]}"; do
	#if expr $((i)) "<" 1; then
		echo -e "${YELLOW}${BOLD}Test $((i+1)) : ${CLASSIC}${YELLOW}${TESTS[i]}${CLASSIC}"
		${TESTS[i]} > out.txt
		if diff out.txt ./nt/$((i+1)).txt; then
			echo -e "${GREEN}${BOLD}SUCCESS!${CLASSIC}\n"
		else
			echo -e "${RED}${BOLD}FAILED!${CLASSIC}\n"
		fi
	#fi
done
rm -rf out.txt
