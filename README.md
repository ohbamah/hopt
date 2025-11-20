 ## Documentation website

[documentation here](https://hopt-doc.fr)

## Installation (limited)

### Linux

```bash
  wget https://github.com/ohbamah/hopt/releases/latest/download/libhopt-linux-x64.tar.gz
```

### Macos

```bash
  wget https://github.com/ohbamah/hopt/releases/latest/download/libhopt-macos-x64.tar.gz
```

### Windows 10/11

```bash
  curl -O https://github.com/ohbamah/hopt/releases/latest/download/libhopt-windows-x64.zip
```

or

```bash
  Invoke-WebRequest https://github.com/ohbamah/hopt/releases/latest/download/libhopt-windows-x64.zip -OutFile libhopt.zip
```

## FAQ

#### Is HOPT more faster than argp ?
Yes it is a little faster than argp on average, but it all depends on how you will use it. It's proportionally.

#### Is HOPT easier to use than argp ?
Of course! Just take a look at the "Usage/Examples" section below.

#### Is HOPT compatible with different OS ?
The library follows the POSIX standard, so in theory it will be compatible with at least Windows 10/11, Linux and MacOS.

## Comparison with ARGP

#### Get all items

With HOPT, you can customize everything while keeping extreme control over option management.

| Features                        | HOPT     | ARGP     | GETOPT |
| :------------------------------ | :------- | :-------- | :----- |
| Parse program options           | ​✅       | ✅       | ✅     |
| Parse option arguments          | ​✅       | ✅       | ✅     |
| Sort argument values            | ​✅       | ❌       | ❌     |
| Easy and readable usage         | ​✅       | 〰️       | ​✅     |
| Simple optional features        | ​✅       | ❌       | ❌     |
| Advanced logic support          | ​✅       | ​✅       | ❌     |
| Returns error codes             | ​✅       | ​✅       | ​✅     |
| Supports short and long options | ​✅       | ​✅       | ​✅     |
| Supports callbacks              | ​✅       | ​✅       | ❌     |
| Custom help menu setup          | ​✅       | 〰️       | ❌     |
| Auto-generated help menu        | ​✅       | ​✅       | ❌     |
| Manages sub-command options     | ​✅       | ​〰️       | ❌     |
| Manages embedded sub-commands   | ​✅       | ​❌       | ❌     |
| Automatic error handling        | 〰️       | ​✅       | 〰️     |
| POSIX (OS compatibilities)      | ✅       | ​❌       | ✅     |
| Language translation            | ❌       | ​✅       | ❌     |
| Variadic option arguments       | ✅       | ​✅       | ❌     |


## Usage/Examples

This is short exemple, but you can have a look at benchmark/hopt_usage.c for a more complexe usage of HOPT.

```c
#include <hopt.h>
#include <stdio.h>
#include <stdlib.h>

struct t_opt
{
  char* name;
  int count;
  // ...
};

int main(int ac, char** av)
{
    t_opt  options = {0};

    hopt_add_option("-name", 1, HOPT_TYPE_STR, &options.name, "Name of the subject.");
    hopt_add_option("c=-count=n", 1, HOPT_TYPE_INT, &options.count, "Max count possible.");
    /*
      ...
    */
    int count = hopt(ac, av); // Main function
    if (hopt_help_has_been_called())
        return (0);
    else if (count == -1)
    {
        char*   error = hopt_strerror();
        printf("%s\n", error);
        free(error);
        /*
            Handling errors
        */
        return (1);
    }
    ac -= (count + 1);
    av += (count + 1); // If count != 1, count will be equal to the number of arguments (which are options) to be skipped. Useful only if 'hopt_disable_sort()' has not been called.
    /*
        Rest of the program...
    */
    return (0);
}
```

## Edge cases:

- If an option with variadic arguments if followed by a subcommand, the subcommand will be interpreted as an argument for the option.

## Todo:

- Handle type wrapper like IPv4, IPv6, numeric range, file, ...

## Authors

If you have any feedback, please reach out to us at ohbama@posteo.net.

- [@ohbama](https://github.com/ohbamah)

[![MIT Licence](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
