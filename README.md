Caramel - C Compiler with syntactic sugar
========================

Caramel is a compiler of a C language subset. Caramel is divided into 2 parts :
- The backend, which extracts an Intermediate Representation (IR)
- The frontend, which compile the IR into full assembly language (x86 ASM)

## Authors

Caramel has been created by KALATE Team:
- Kévin Dumanoir (kevin.dumanoir@insa-lyon.fr)
- Alexandre Gourgaud (alexandre.gourgaud@insa-lyon.fr)
- Thomas Lacroix (<thomaslacroix@free.fr>)
- Aurore Loiseau (aurore.loiseau@insa-lyon.fr)
- Elise Petit (elise.petit@insa-lyon.fr)
- Loïc Rouquette (<rouquette.loic@gmail.com>)

for the CS department - INSA Lyon 2018.


## How to download the compiler

The library uses GoogleTest for its tests, using git sub-modules. Hence, you have to use one of the following to fetch the GoogleTest sources:

**TODO : Update this part**

```shell
# Git clone with SSH
git clone --recursive git@gitlab.com:hexanome-kalate/pld-comp.git

# Git clone with HTTPS
git clone --recursive https://gitlab.com/hexanome-kalate/pld-comp.git

# If already cloned
git submodule update --init --recursive
```


## How to run the examples

**Caramel requires a C++14 compiler, at least GCC 6, and CMake 3.8.**

```shell
# Generate the Makefile using CMake
mkdir build
cd build
cmake ..

# Todo : make some examples
```

## Chef commands :
```bash
usage: chef.py [-h] [--verbose | --quiet] {clean,build,test} ...

The Caramel Jack of all trades.

optional arguments:
  -h, --help          show this help message and exit
  --verbose, -v       increase the verbosity (repeat for even more verbosity)
  --quiet, -q         decrease the verbosity (repeat for less verbosity)

Available commands:
  {clean,build,test}
    clean             Ask the Chef to clean up his workplace.
    build             Make the Chef cook some Caramel.
    test              Test the Caramel quality.
```

### Example for executiong all test
```bash
# usage: chef.py test [-h] {grammar,semantic,all} ...
#
# optional arguments:
#   -h, --help            show this help message and exit
#
# Available sub-commands:
#   {grammar,semantic,all}
#     grammar             Test the Caramel grammar.
#     semantic            Test the Caramel semantic analysis.
#     all                 Run all tests.

./chef.py test all -bw
```
Will brew the grammar, build the project and execute all the tests.

Tests marked as `[+]` are valid tests that must succed.
Tests markes as `[-]` are unvalid tests that must failed.

## Grammar
- [Grammar](doc/grammar.md)

## Semantic
- [Semantic](doc/semantic.md)

## Phases
- [Phase 5.1](doc/p_51.md)
- [Phase 5.2](doc/p_52.md)
- [Phase 5.3](doc/p_53.md)
- [Phase 5.4](doc/p_54.md)
- [Phase 5.5](doc/p_55.md)
- [Phase 5.6](doc/p_56.md)
- [Phase 5.7](doc/p_57.md)
- [Phase 5.8](doc/p_58.md)
- [Phase 5.9](doc/p_59.md)
- [Phase 5.10](doc/p_510.md)
- [Phase 5.11](doc/p_511.md)
- [Phase 5.12](doc/p_512.md)
- [Phase 5.13](doc/p_513.md)
- [Phase 5.14](doc/p_514.md)
- [Phase 5.15](doc/p_515.md)

## What's working (not exhaustive)?
- [x] variable definition
- [x] function definition
- [x] array definition
- [x] more than 6 args function call
- [x] recursive function call
- [x] operators priority
- [x] complex nested expression (ex: foo(1 || 4 & 2) || 1>2 || (1==1 && 4>3))
- [x] if ... then ... else
- [x] while block
- [x] do while block
- [x] for block
- [ ] real stack size (in prolog)
- [ ] correct types handling (int8_t, int16_t -> int32_t)
