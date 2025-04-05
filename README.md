## Installation

### Linux

To install requirements and repository:

```bash
  apt-get install -y git clang make
  git clone https://github.com/ohbamah/hopt.git
```

To copy HOPT to the include path (do it in the repo cloned AND need to be root):

```bash
  sudo cp ./libhopt.a /lib/ || cp ./libhopt.a /usr/lib/
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

To copy HOPT to the include path (do it in the repo cloned):

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
Of course we do! I will let you have a look at the DEMO.

#### Is HOPT compatible in different OS ?
The library follows the POSIX standard, so in theory it will be compatible with at least Windows 10/11, Linux and MacOS.

#### Does HOPT have many useful features ?
Obviously, it has more features than argp and you have control over what you do.

## Comparison with ARGP

#### Get all items

With HOPT, you can customize everything while keeping extreme control over option management.

| Features                        | HOPT     | ARGP     | GETOPT |
| :------------------------------ | :------- | :--------| :----- |
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
| Automatic error handling        | ❌       | ​✅       | ❌     |


## Usage/Examples

This is short exemple, but you can have a look at benchmark/hopt_usage.c for a more complexe usage of HOPT.

```c
#include <hopt.h>
#include <stdio.h>
#include <stdlib.h>

int main(int ac, char** av)
{
    hopt_allow_undef(); // Allow undefined options
    hopt_allow_redef(TRUE); // Allow options redefinition (with overwriting or not)
    hopt_end_on_arg(); // End parsing while a non-option argument appears
    hopt_disable_sort(); // Disable AV sorting

    hopt_reset(); // Reset all features (redef/undef/...)
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


## Authors

If you have any feedback, please reach out to us at ohbama@posteo.net.

- [@ohbama](https://github.com/ohbamah)

[![MIT Licence](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
