#include "unit_tester.hpp"
#include <cstring>

int tests_passed = 0;
int tests_total = 0;

# define CREATE_ARGS(...) {"./prog", __VA_ARGS__, NULL}
# define ARGS_SIZE(tab) sizeof(tab)/sizeof(tab[0]) - 1

void
check_expectation(bool condition, const char* description)
{
    tests_total++;
    if (condition)
    {
        printf("        \033[32m%s\033[0m\n", description);
        tests_passed++;
    } else
        printf("        \033[31m%s\033[0m\n", description);
}

void
check_expectation_str_eq(const char* str1, const char* str2)
{
    check_expectation(!strcmp(str1, str2), (str1 + std::string(" == ") + str2).data());
}

void
check_expectation_str_ne(const char* str1, const char* str2)
{
    check_expectation(!strcmp(str1, str2), (str1 + std::string(" != ") + str2).data());
}

void*
subcommand(void* arg)
{
    char* ptr = *(char**)arg;
    // ptr = 
    return ptr;
}

int main(void)
{
    printf("🚀 HOPT Unit Tests\n");
    printf("==================\n");

    int     size;
    int     result;

    void*   mock = (void*)new long;
    bool    bool_mock = 0;
    char    char_mock = 0;
    short   short_mock = 0;
    int     int_mock = 0;
    long    long_mock = 0;
    char*   str_mock[24] = {0};

        // TODO with allow_undef ...
        // TODO with undefined options ...

    // CONCATENATED SHORT OPTIONS

    TESTS("Concatenated short options")
    {
        DESCRIBE("should works")
        {
            hopt_free();
            hopt_reset();

            bool    o_mock = false;
            bool    v_mock = false;
            bool    p_mock = false;
            bool    d_mock = false;
            bool    s_mock = false;
            int     a_mock = false;

            char* args[] = CREATE_ARGS("-ovdsa", "23");
            size = ARGS_SIZE(args);

            hopt_add_option("o", 0, 0, &o_mock, NULL);
            hopt_add_option("v", 0, 0, &v_mock, NULL);
            hopt_add_option("p", 0, 0, &p_mock, NULL);
            hopt_add_option("d", 0, 0, &d_mock, NULL);
            hopt_add_option("s", 0, 0, &s_mock, NULL);
            hopt_add_option("a", 1, HOPT_TYPE_INT, &a_mock, NULL);
            result = hopt(size, args);

            printf("%d - %s\n", result, hopt_strerror());
            EXPECTS(result == 2);
            EXPECTS(o_mock == true);
            EXPECTS(v_mock == true);
            EXPECTS(p_mock == false);
            EXPECTS(d_mock == true);
            EXPECTS(s_mock == true);
            EXPECTS(a_mock == 23);
        }

        CONTEXT("with arguments")
        {

            IT("should returns an error if there is a wrong order")
            {
                hopt_free();
                hopt_reset();

                bool    mock = false;

                char* args[] = CREATE_ARGS("-ovdas", "23");
                size = ARGS_SIZE(args);

                hopt_add_option("o", 0, 0, &mock, NULL);
                hopt_add_option("v", 0, 0, &mock, NULL);
                hopt_add_option("p", 0, 0, &mock, NULL);
                hopt_add_option("d", 0, 0, &mock, NULL);
                hopt_add_option("s", 0, 0, &mock, NULL);
                hopt_add_option("a", 1, HOPT_TYPE_INT, &mock, NULL);
                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option -a is in bad order, he need argument(s) but is not in last position in a string of shorts options.");
            }
        

            IT("should works with correct order")
            {
                hopt_free();
                hopt_reset();

                bool    mock = false;

                char* args[] = CREATE_ARGS("-ovdas", "23");
                size = ARGS_SIZE(args);

                hopt_add_option("o", 0, 0, &mock, NULL);
                hopt_add_option("v", 0, 0, &mock, NULL);
                hopt_add_option("p", 0, 0, &mock, NULL);
                hopt_add_option("d", 0, 0, &mock, NULL);
                hopt_add_option("s", 0, 0, &mock, NULL);
                hopt_add_option("a", 1, HOPT_TYPE_INT, &mock, NULL);
                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option -a is in bad order, he need argument(s) but is not in last position in a string of shorts options.");
            }

            IT("misses them")
            {
                hopt_free();
                hopt_reset();

                bool    mock = false;

                char* args[] = CREATE_ARGS("-ovdsa");
                size = ARGS_SIZE(args);

                hopt_add_option("o", 0, 0, &mock, NULL);
                hopt_add_option("v", 0, 0, &mock, NULL);
                hopt_add_option("p", 0, 0, &mock, NULL);
                hopt_add_option("d", 0, 0, &mock, NULL);
                hopt_add_option("s", 0, 0, &mock, NULL);
                hopt_add_option("a", 1, HOPT_TYPE_INT, &mock, NULL);
                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option -a miss argument(s).");
            }
        }        
    }

    // ADD OPTIONS
    TESTS("Add options")
    {
        CONTEXT("with no argument")
        {
            IT("expects nothing")
            {
                hopt_free();
                hopt_reset();

                char* args[] = CREATE_ARGS("-o");
                size = ARGS_SIZE(args);

                hopt_add_option((char*)"o", 0, 0, mock, NULL);
                result = hopt(size, args);

                EXPECTS(result == 1);
            }

            IT("expects them")
            {
                hopt_free();
                hopt_reset();

                char* args[] = CREATE_ARGS("frr", "-o");
                size = ARGS_SIZE(args);

                hopt_add_option((char*)"o", 2, 0, mock, NULL);
                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option -o miss argument(s).");
            }
        }

        CONTEXT("with arguments starting by `-`")
        {
            IT("numerics")
            {
                hopt_free();
                hopt_reset();

                char* args[] = CREATE_ARGS("-o", "-2321", "32", "-349");
                size = ARGS_SIZE(args);

                hopt_add_option((char*)"o", 3, HOPT_TYPE_INT, mock, NULL);
                result = hopt(size, args);

                EXPECTS(result == 4);
            }

            IT("strings")
            {
                hopt_free();
                hopt_reset();

                char* args[] = CREATE_ARGS("frr", "-o", "-d", "--miaou");
                size = ARGS_SIZE(args);

                hopt_add_option((char*)"o", 2, 0, mock, NULL);
                result = hopt(size, args);

                EXPECTS(result == 3);
            }
        }

        CONTEXT("with string arguments")
        {
            hopt_free();
            hopt_reset();

            char* args[] = CREATE_ARGS("frr", "--str", "all good ?\n", "yoo", "mister white", "ahah");
            size = ARGS_SIZE(args);

            hopt_add_option((char*)"-str", 3, 0, str_mock, NULL);
            result = hopt(size, args);

            EXPECTS(result == 4);
            EXPECTS_STR_EQ(str_mock[0], "all good ?\n");
            EXPECTS_STR_EQ(str_mock[1], "yoo");
            EXPECTS_STR_EQ(str_mock[2], "mister white");
            EXPECTS(str_mock[3] == NULL);
            memset(str_mock, 0, sizeof(str_mock));
        }

        CONTEXT("with char arguments")
        {
            hopt_free();
            hopt_reset();

            char* args[] = CREATE_ARGS("frr", "-c", "128", "all good ?\n", "yoo", "mister white", "ahah");
            size = ARGS_SIZE(args);

            hopt_add_option((char*)"c", 1, HOPT_TYPE_CHAR, &char_mock, NULL);
            result = hopt(size, args);

            EXPECTS(result == 2);
            EXPECTS(char_mock == -128);
        }

        CONTEXT("with short arguments")
        {
            hopt_free();
            hopt_reset();

            char* args[] = CREATE_ARGS("frr", "-c", "-32769", "all good ?\n", "yoo", "mister white", "ahah");
            size = ARGS_SIZE(args);

            hopt_add_option((char*)"c", 1, HOPT_TYPE_SHORT, &short_mock, NULL);
            result = hopt(size, args);

            EXPECTS(result == 2);
            EXPECTS(short_mock == 32767);
        }

        CONTEXT("with int arguments")
        {
            hopt_free();
            hopt_reset();

            char* args[] = CREATE_ARGS("frr", "-c", "-214123432", "all good ?\n", "yoo", "mister white", "ahah");
            size = ARGS_SIZE(args);

            hopt_add_option((char*)"c", 1, HOPT_TYPE_INT, &int_mock, NULL);
            result = hopt(size, args);

            EXPECTS(result == 2);
            EXPECTS(int_mock == -214123432);
        }

        CONTEXT("with long arguments")
        {
            hopt_free();
            hopt_reset();

            char* args[] = CREATE_ARGS("frr", "--count", " -32769", "all good ?\n", "yoo", "mister white", "ahah");
            size = ARGS_SIZE(args);

            hopt_add_option((char*)"-count", 0, 0, &short_mock, NULL);
            result = hopt(size, args);

            EXPECTS(result == 1);
        }

        CONTEXT("with variadic arguments")
        {
            hopt_free();
            hopt_reset();

            char* mock[10];

            char* args[] = CREATE_ARGS("-v", "123", "321", "324", "543", "0123", "9543", "483", "jtsrnfk", "sekfmse", "fse");
            size = ARGS_SIZE(args);

            hopt_add_option((char*)"v", HOPT_VARIADIC_ARGUMENTS, 0, mock, NULL);
            result = hopt(size, args);

            EXPECTS(result == 11);
            printf("result %d\n", result);
            EXPECTS_STR_EQ(mock[0], "123");
            EXPECTS_STR_EQ(mock[1], "321");
            EXPECTS_STR_EQ(mock[2], "324");
            EXPECTS_STR_EQ(mock[3], "543");
            EXPECTS_STR_EQ(mock[4], "0123");
            EXPECTS_STR_EQ(mock[5], "9543");
            EXPECTS_STR_EQ(mock[6], "483");
            EXPECTS_STR_EQ(mock[7], "jtsrnfk");
            EXPECTS_STR_EQ(mock[8], "sekfmse");
            EXPECTS_STR_EQ(mock[9], "fse");
        }
    }

    // UNDEFINED OPTIONS
    TESTS("Undefined options")
    {
        CONTEXT("when it's short options")
        {
            IT("should fail")
            {
                hopt_free();
                hopt_reset();

                char* args[] = CREATE_ARGS("test", "-x");
                size = ARGS_SIZE(args);

                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option -x is undefined.");
            }
        }
        CONTEXT("when it's long options")
        {
            IT("should fail")
            {
                hopt_free();
                hopt_reset();

                char* args[] = CREATE_ARGS("test", "--xav");
                size = ARGS_SIZE(args);

                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option --xav is undefined.");
            }
        }
        CONTEXT("with complexe usages")
        {
            IT("should fail at first arg")
            {
                hopt_free();
                hopt_reset();

                char* args[] = CREATE_ARGS("-x");
                size = ARGS_SIZE(args);

                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option -x is undefined.");
            }
            IT("should fail with many options")
            {
                hopt_free();
                hopt_reset();

                char* args[] = CREATE_ARGS("Yoo la team", "x", "hoho=213", "-p=3", "--count", "feur", "-x", "last");
                size = ARGS_SIZE(args);

                hopt_add_option((char*)"p", 1, HOPT_TYPE_INT, mock, NULL);
                hopt_add_option((char*)"-count", 1, 0, mock, NULL);
                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option -x is undefined.");
            }
        }
    }

    // REDEFINED OPTIONS
    TESTS("Redefined options")
    {
        CONTEXT("when it's short options")
        {
            IT("should fail")
            {
                hopt_free();
                hopt_reset();

                char* args[] = CREATE_ARGS("test", "-x", "yoo", "miam", "-x", "fra");
                size = ARGS_SIZE(args);

                hopt_add_option((char*)"x", 0, 0, mock, NULL);
                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option -x is redefined.");
            }
        }

        CONTEXT("when it's long options")
        {
            IT("should fail")
            {
                hopt_free();
                hopt_reset();

                char* args[] = CREATE_ARGS("test","test", "--xav", "yoo", "miam", "--xav", "fra");
                size = ARGS_SIZE(args);

                hopt_add_option((char*)"-xav", 0, 0, mock, NULL);
                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option --xav is redefined.");
            }
        }

        CONTEXT("with complexe usages")
        {
            IT("should fail at first arg")
            {
                hopt_free();
                hopt_reset();

                char* args[] = CREATE_ARGS("-x", "-x");
                size = ARGS_SIZE(args);

                hopt_add_option((char*)"x", 0, 0, mock, NULL);
                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option -x is redefined.");
            }

            IT("should fail with many options")
            {
                hopt_free();
                hopt_reset();

                char* args[] = CREATE_ARGS("Yoo la team", "x", "hoho=213", "-p=3", "--count", "feur", "-x", "last", "-x");
                size = ARGS_SIZE(args);

                hopt_add_option((char*)"x", 0, 0, mock, NULL);
                hopt_add_option((char*)"p", 1, HOPT_TYPE_INT, mock, NULL);
                hopt_add_option((char*)"-count", 1, 0, mock, NULL);
                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option -x is redefined.");
            }
        }

        CONTEXT("with allow_redef()")
        {
            IT("should allow with long option")
            {
                hopt_free();
                hopt_reset();

                char*   str_mock = NULL;

                char* args[] = CREATE_ARGS("Yoo la team", "x", "hoho=213", "-p=3", "--count", "feur", "-x", "last", "-x");
                size = ARGS_SIZE(args);

                hopt_allow_redef(false);
                hopt_add_option((char*)"x", 0, 0, mock, NULL);
                hopt_add_option((char*)"p", 1, HOPT_TYPE_INT, mock, NULL);
                hopt_add_option((char*)"-count", 1, 0, &str_mock, NULL);
                result = hopt(size, args);

                EXPECTS(result == 5);
                EXPECTS_STR_EQ(str_mock, "feur");
            }
        }
    }

    // AV sorted
    TESTS("AV sorted")
    {
        CONTEXT("with options")
        {
            IT("should sort options")
            {
                hopt_free();
                hopt_reset();

                char* args[] = CREATE_ARGS("file1", "file2", "--option1", "file3", "--option2", "arg option", "file4", "--last_option");
                size = ARGS_SIZE(args);

                hopt_add_option((char*)"-option1", 0, 0, mock, NULL);
                hopt_add_option((char*)"-option2", 1, 0, mock, NULL);
                hopt_add_option((char*)"-last_option", 0, 0, mock, NULL);
                result = hopt(size, args);

                EXPECTS(result == 4);
                EXPECTS_STR_EQ(args[0], "./prog");
                EXPECTS_STR_EQ(args[1], "--option1");
                EXPECTS_STR_EQ(args[2], "--option2");
                EXPECTS_STR_EQ(args[3], "arg option");
                EXPECTS_STR_EQ(args[4], "--last_option");
                EXPECTS_STR_EQ(args[5], "file1");
                EXPECTS_STR_EQ(args[6], "file2");
                EXPECTS_STR_EQ(args[7], "file3");
                EXPECTS_STR_EQ(args[8], "file4");
                EXPECTS(args[9] == NULL);
            }
        }

        CONTEXT("with disable_sort()")
        {
            IT("should not sort")
            {
                hopt_free();
                hopt_reset();

                char* args[] = CREATE_ARGS("file1", "file2", "--option1", "file3", "--option2", "arg option", "file4", "--last_option");
                size = ARGS_SIZE(args);

                hopt_disable_sort();
                hopt_add_option((char*)"-option1", 0, 0, mock, NULL);
                hopt_add_option((char*)"-option2", 1, 0, mock, NULL);
                hopt_add_option((char*)"-last_option", 0, 0, mock, NULL);
                result = hopt(size, args);

                EXPECTS(result == 4);
                EXPECTS_STR_EQ(args[0], "./prog");
                EXPECTS_STR_EQ(args[1], "file1");
                EXPECTS_STR_EQ(args[2], "file2");
                EXPECTS_STR_EQ(args[3], "--option1");
                EXPECTS_STR_EQ(args[4], "file3");
                EXPECTS_STR_EQ(args[5], "--option2");
                EXPECTS_STR_EQ(args[6], "arg option");
                EXPECTS_STR_EQ(args[7], "file4");
                EXPECTS_STR_EQ(args[8], "--last_option");
                EXPECTS(args[9] == NULL);
            }
        }

        CONTEXT("without options")
        {
            IT("should not sort")
            {
                hopt_free();
                hopt_reset();

                char* args[] = CREATE_ARGS("file1", "file2", "file3", "file4");
                size = ARGS_SIZE(args);

                hopt_add_option((char*)"-option1", 0, 0, mock, NULL);
                hopt_add_option((char*)"-option2", 1, 0, mock, NULL);
                hopt_add_option((char*)"-last_option", 0, 0, mock, NULL);
                result = hopt(size, args);

                EXPECTS(result == 0);
                EXPECTS_STR_EQ(args[0], "./prog");
                EXPECTS_STR_EQ(args[1], "file1");
                EXPECTS_STR_EQ(args[2], "file2");
                EXPECTS_STR_EQ(args[3], "file3");
                EXPECTS_STR_EQ(args[4], "file4");
                EXPECTS(args[5] == NULL);
            }
        }

        CONTEXT("without AV")
        {
            IT("should do nothing")
            {
                hopt_free();
                hopt_reset();

                char* args[] = CREATE_ARGS(NULL);
                size = ARGS_SIZE(args);

                hopt_add_option((char*)"-y", 0, 0, mock, NULL);
                result = hopt(size, args);

                EXPECTS(result == 0);
            }
        }
    }

    TESTS("Subcommands")
    {
        char* returns;

        CONTEXT("check order")
        {

        }

        CONTEXT("without error")
        {
            IT("should work")
                {
                hopt_free();
                hopt_reset();

                char* arg_test = "abcd";

                char* args[] = CREATE_ARGS("run", "build", "-f");
                size = ARGS_SIZE(args);

                hopt_subcmd((char*)"run", subcommand, &arg_test, (void**)&returns)
                {
                    hopt_subcmd((char*)"build", subcommand, &arg_test, (void**)&returns)
                    {
                        hopt_add_option((char*)"f", 0, 0, mock, NULL);
                    }
                }
                result = hopt(size, args);

                EXPECTS(result == 3);
            }

            IT("should work")
            {
                hopt_free();
                hopt_reset();

                char* arg_test = "abcd";

                char* args[] = CREATE_ARGS("run", "-f", "build");
                size = ARGS_SIZE(args);

                hopt_subcmd((char*)"run", subcommand, &arg_test, (void**)&returns)
                {
                    hopt_add_option((char*)"f", 0, 0, mock, NULL);
                    hopt_subcmd((char*)"build", subcommand, &arg_test, (void**)&returns)
                    {
                    }
                }
                result = hopt(size, args);

                EXPECTS(result == 3);
            }

            IT("should work")
            {
                hopt_free();
                hopt_reset();

                char* arg_test = "abcd";

                char* args[] = CREATE_ARGS("-f", "run", "build");
                size = ARGS_SIZE(args);

                hopt_add_option((char*)"f", 0, 0, mock, NULL);
                hopt_subcmd((char*)"run", subcommand, &arg_test, (void**)&returns)
                {
                    hopt_subcmd((char*)"build", subcommand, &arg_test, (void**)&returns)
                    {
                    }
                }
                result = hopt(size, args);

                EXPECTS(result == 3);
            }
        }

        CONTEXT("with undefined options")
        {
            IT("should returns an error")
            {
                hopt_free();
                hopt_reset();

                char* arg_test = "abcd";

                char* args[] = CREATE_ARGS("run", "-f", "build");
                size = ARGS_SIZE(args);

                hopt_subcmd((char*)"run", subcommand, &arg_test, (void**)&returns)
                {
                    hopt_subcmd((char*)"build", subcommand, &arg_test, (void**)&returns)
                    {
                        hopt_add_option((char*)"f", 0, 0, mock, NULL);
                    }
                }
                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option -f is undefined.");
            }

            IT("should returns an error")
            {
                hopt_free();
                hopt_reset();

                char* arg_test = "abcd";

                char* args[] = CREATE_ARGS("-f", "run", "build");
                size = ARGS_SIZE(args);

                hopt_subcmd((char*)"run", subcommand, &arg_test, (void**)&returns)
                {
                    hopt_subcmd((char*)"build", subcommand, &arg_test, (void**)&returns)
                    {
                        hopt_add_option((char*)"f", 0, 0, mock, NULL);
                    }
                }
                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option -f is undefined.");
            }


            IT("should returns an error")
            {
                hopt_free();
                hopt_reset();

                char* arg_test = "abcd";

                char* args[] = CREATE_ARGS("-f", "run", "build");
                size = ARGS_SIZE(args);

                hopt_subcmd((char*)"run", subcommand, &arg_test, (void**)&returns)
                {
                    hopt_add_option((char*)"f", 0, 0, mock, NULL);
                    hopt_subcmd((char*)"build", subcommand, &arg_test, (void**)&returns)
                    {
                    }
                }
                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option -f is undefined.");
            }

            IT("should returns an error")
            {
                hopt_free();
                hopt_reset();

                char* arg_test = "abcd";

                char* args[] = CREATE_ARGS("run", "build", "-f");
                size = ARGS_SIZE(args);

                hopt_subcmd((char*)"run", subcommand, &arg_test, (void**)&returns)
                {
                    hopt_add_option((char*)"f", 0, 0, mock, NULL);
                    hopt_subcmd((char*)"build", subcommand, &arg_test, (void**)&returns)
                    {
                    }
                }
                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option -f is undefined.");
            }

            IT("should returns an error")
            {
                hopt_free();
                hopt_reset();

                char* arg_test = "abcd";

                char* args[] = CREATE_ARGS("run", "build", "-f");
                size = ARGS_SIZE(args);

                hopt_add_option((char*)"f", 0, 0, mock, NULL);
                hopt_subcmd((char*)"run", subcommand, &arg_test, (void**)&returns)
                {
                    hopt_subcmd((char*)"build", subcommand, &arg_test, (void**)&returns)
                    {
                    }
                }
                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option -f is undefined.");
            }

            IT("should returns an error")
            {
                hopt_free();
                hopt_reset();

                char* arg_test = "abcd";

                char* args[] = CREATE_ARGS("run", "-f", "build");
                size = ARGS_SIZE(args);

                hopt_add_option((char*)"f", 0, 0, mock, NULL);
                hopt_subcmd((char*)"run", subcommand, &arg_test, (void**)&returns)
                {
                    hopt_subcmd((char*)"build", subcommand, &arg_test, (void**)&returns)
                    {
                    }
                }
                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option -f is undefined.");
            }
        }

        CONTEXT("with redefined options")
        {
            IT("should returns an error")
            {
                hopt_free();
                hopt_reset();

                char* arg_test = "abcd";

                char* args[] = CREATE_ARGS("run", "-f", "build");
                size = ARGS_SIZE(args);

                hopt_subcmd((char*)"run", subcommand, &arg_test, (void**)&returns)
                {
                    hopt_subcmd((char*)"build", subcommand, &arg_test, (void**)&returns)
                    {
                        hopt_add_option((char*)"f", 0, 0, mock, NULL);
                    }
                }
                result = hopt(size, args);

                EXPECTS(result == -1);
                EXPECTS_STR_EQ(hopt_strerror(), "hopt: option -f is undefined.");
            }
        }
    }
    
    delete (long*)mock;
    
    // Résumé final
    printf("\n📊 RESUME\n");
    printf("=========\n");
    printf("Tests passed: %d/%d\n", tests_passed, tests_total);
    
    if (tests_passed == tests_total) {
        printf("🎉 All tests are passed !\n");
        return 0;
    } else {
        printf("💥 %d test(s) failed !\n", tests_total - tests_passed);
        return 1;
    }
}
