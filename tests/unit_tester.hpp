#ifndef UNIT_TESTER_HPP
#define UNIT_TESTER_HPP

# include <string>
# include <cstdio>

extern "C" {
    # include "../includes/hopt.h"
}

# define CUSTOM_SCOPE \
    for (bool ended = true ; ended ; ended = false)

# define CUSTOM_SCOPE_WITH_ENTRIES(first, last) \
    for (bool ended = (first, true) ; ended ; ended = (last, false))

# define DEBUG(var)             printf("%s: %d\n", #var, var);
# define DEBUG_STR(var)         printf("%s: %s\n", #var, var);

# define TESTS(title)           CUSTOM_SCOPE_WITH_ENTRIES(printf("\n🧪 " title "\n"), 0)
# define CONTEXT(title)         CUSTOM_SCOPE_WITH_ENTRIES(printf("  📁 " title "\n"), 0)
# define IT(title)              CUSTOM_SCOPE_WITH_ENTRIES(printf("      ▶  " title "\n"), 0)

# define DESCRIBE(title)        CUSTOM_SCOPE_WITH_ENTRIES(printf("  ▶  " title "\n"), 0)

# define EXPECTS(expectation)           check_expectation(expectation, #expectation)
# define EXPECTS_STR_EQ(str1, str2)     check_expectation_str_eq(str1, str2)
# define EXPECTS_STR_NE(str1, str2)     check_expectation_str_ne(str1, str2)

// Variables globales pour les statistiques
extern int tests_passed;
extern int tests_total;

// Fonction utilitaire pour vérifier les attentes
void
check_expectation(bool condition, const char* description);

void
check_expectation_str_eq(bool condition, const char* description);

void
check_expectation_str_ne(bool condition, const char* description);

// Macros utilitaires pour les tests HOPT
# define SETUP_HOPT() do { hopt_reset(); } while(0)
# define ADD_OPTION(opt, argc, type, ptr) hopt_add_option((char*)opt, argc, type, ptr, NULL)
# define RUN_HOPT(av_array) hopt(sizeof(av_array)/sizeof(av_array[0]), (char**)av_array)
# define EXPECT_ERROR(expected_msg) (hopt_strerror() && strstr(hopt_strerror(), expected_msg) != NULL)

#endif
