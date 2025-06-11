#include "includes/hopt.h"
#include <stdio.h>

int cb(int ac, char** av, void* ptr)
{
    return (-1);
}

int main(int ac, char** av)
{
    hopt_add_option("I", 1, HOPT_TYPE_CB, cb, (void*)0, (void*)0);
    int c = hopt(ac, av);
    return (0);
}
