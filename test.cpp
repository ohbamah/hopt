#include <stdlib.h>
#include <stdio.h>

extern "C" {
    #include "includes/hopt.h"
}

int main(int ac, char** av)
{
    void* mock = (void*)new long;
    short   feur = 0;
    int     result = 0;

    hopt_add_option((char*)"o", 2, HOPT_TYPE_SHORT, &feur, NULL);
    result = hopt(ac, av);

    printf("%d\n", (short)feur);
    // delete (long*)mock;

    printf("%s\n", hopt_strerror());
    return result;
}

