## Installation

### Linux

To install requirements and repository:

```bash
  apt-get install -y git clang make
  git clone https://github.com/ohbamah/hopt.git
```

To copy HOPT to the include path (Make sure you have root access and run this from the root of the repo):

```bash
  sudo cp ./libhopt.a /lib/ || sudo cp ./libhopt.a /usr/lib/
  sudo cp ./includes/hopt.h /usr/include/
```

All-In-One:

```bash
  git clone https://github.com/ohbamah/hopt.git && cd ./hopt/ && make && sudo cp ./libhopt.a /lib/ && sudo cp ./includes/hopt.h /usr/include/ && cd .. ; rm ./hopt -rf
```

### MacOS

To install requirements and repository:

```bash
  brew install git clang make
  git clone https://github.com/ohbamah/hopt.git
```

To copy HOPT to the include path (Make sure you have root access and run this from the root of the repo):

```bash
  sudo mkdir -p /usr/local/lib/
  sudo cp ./libhopt.a /usr/local/lib/
  sudo mkdir -p /usr/local/include/
  sudo cp ./includes/hopt.h /usr/local/inlude/
```

All-In-One:

```bash
  sudo mkdir -p /usr/local/lib/ && sudo mkdir -p /usr/local/include/ && git clone https://github.com/ohbamah/hopt.git && cd ./hopt/ && make &&  sudo cp ./libhopt.a /usr/local/lib/ && sudo cp ./includes/hopt.h /usr/local/inlude/ && cd .. ; rm ./hopt -rf
```

### Windows 10/11

To install repository and requirements (in the releases section):

- Download the latest-stable version in the repo github page releases section.
- Run .\WindowsHOPT\1-ChocoInstall.bat | To install Chocolatey.
- Run .\WindowsHOPT\2-Requirements.bat | To install git, clang and make.
- Go to the directory where you want to have the repo and run .\WindowsHOPT\3-CloneGit.bat | To clone repo.

To copy HOPT to the include Clang/LLVM path (not working with MSVC):

- Run .\WindowsHOPT\4-LLVMPATH.bat


## FAQ

#### Is HOPT more faster than argp ?
Yes it is a little faster than argp on average.

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
| Parse option arguments          | ​✅       | ✅       | ❌     |
| Sort argument values            | ​✅       | ❌       | ❌     |
| Easy and readable usage         | ​✅       | ❌       | ​✅     |
| Simple optional features        | ​✅       | ❌       | ❌     |
| Advanced logic support          | ​✅       | ​✅       | ❌     |
| Returns error codes             | ​✅       | ​✅       | ​✅     |
| Supports short and long options | ​✅       | ​✅       | ​✅     |
| Supports callbacks              | ​✅       | ​✅       | ❌     |
| Custom help menu setup          | ​✅       | ❌       | ❌     |
| Auto-generated help menu        | ​✅       | ​✅       | ❌     |
| Manages sub-command options     | ​✅       | ​〰️       | ❌     |
| Automatic error handling        | 〰️       | ​✅       | ❌     |
| POSIX (OS compatibilities)      | ✅       | ​❌       | 〰️     |


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

    hopt_allow_undef(); // Allow undefined options
    hopt_allow_redef(TRUE); // Allow options redefinition (with overwriting or not)
    hopt_end_on_arg(); // End parsing when a non-option argument appears
    hopt_disable_sort(); // Disable AV sorting

    hopt_add_option("-name", 1, HOPT_TYPE_STR, &options.name, "Name of the subject.");
    hopt_add_option("c=-count=n", 1, HOPT_TYPE_INT, &options.count, "Max count possible.");
    /*
      ...
    */
    int count = hopt(ac, av); // Main function
    if (count == -1)
    {
        char*   error = hopt_strerror();
        printf("%s\n", error);
        free(error);
        /*
            Handling errors
        */
        return (1);
    }
    ac -= count;
    av += count; // If count != 1, count will be equal to the number of arguments (which are options) to be skipped. Useful only if 'hopt_disable_sort()' has not been called.
    /*
        Rest of the program...
    */
    return (0);
}
```

## Todo:

- Check wether the option argument is the right type to generate the appropriate error.
- Handle type wrapper like IPv4, IPv6, numeric range, file, ...

## Authors

If you have any feedback, please reach out to us at ohbama@posteo.net.

- [@ohbama](https://github.com/ohbamah)

[![MIT Licence](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
