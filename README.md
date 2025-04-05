## Installation

### Linux

To install repository:

```bash
  apt-get install -y git clang make
  git clone git@github.com:ohbamah/hopt.git && (cd ./hopt/ && rm -rf ./tests/ ; rm -rf ./benchmark/ ; make)
```

To copy HOPT to the include path (do it in the repo cloned AND need to be root):

```bash
  cp ./libhopt.a /lib/ || cp ./libhopt.a /usr/lib/
  cp ./includes/hopt.h /usr/include/
```

### MacOS

To install repository:

```bash
  brew install -y git clang make
  git clone git@github.com:ohbamah/hopt.git && (cd ./hopt/ && rm -rf ./tests/ ; rm -rf ./benchmark/ ; make)
```

To copy HOPT to the include path (do it in the repo cloned AND need to be root):

```bash
#Mettre les commandes
```

### Windows 10/11

To install repository:

```bash
#Mettre les commandes
```

To copy HOPT to the include path (do it in the repo cloned AND need to be root):

```bash
#Mettre les commandes
```

## FAQ

#### Is HOPT more faster than argp ?
Yes it is twice as fast as argp on average.

#### Is HOPT easier to use than argp ?
Of course we do! I will let you have a look at the DEMO.

#### Is HOPT compatible in different OS ?
The library follows the POSIX standard, so in theory it will be compatible with at least Windows 10/11, Linux and MacOS.

#### Does HOPT have many useful features ?
Obviously, it has more features than argp and you have control over what you do.

## Comparison with ARGP

#### Get all items

With HOPT, you can customize everything while keeping extreme control over option management.

| Features                         | HOPT     | ARGP     | GETOPT |
| :------------------------------ | :------- | :--------| :----- |
| Parse program options           | ​✅       | ✅       | ✅     |
| Parse option arguments          | ​✅       | ✅       | ❌     |
| Sort argument values            | ​✅       | ❌       | ❌     |
| Easy and readable usage         | ​✅       | ❌       | ​✅     |
| Simple optional features        | ​✅       | ❌       | ❌     |
| Advanced logic support          | ​✅       | ​✅       | ❌     |
| Returns error codes             | ​✅       | ​✅       | ​✅     |
| Supports short and long options | ​✅       | ​✅       | ​✅     |
| Supports callbacks              | ❌       | ​✅       | ❌     |
| Custom help menu setup          | ​✅       | ❌       | ❌     |
| Auto-generated help menu        | ❌       | ​✅       | ❌     |
| Automatic error handling        | ❌       | ​✅       | ❌     |

## Authors

If you have any feedback, please reach out to us at ohbama@posteo.net.

- [@ohbama](https://github.com/ohbamah)

[![MIT Licence](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
