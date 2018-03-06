Caramel - C Compiler with syntactic sugar
========================

Caramel is a compiler of a C language subset. Caramel is divided into 2 parts :
- The backend, which extracts an Intermediate Representation (IR)
- The frontend, which compile the IR into full assembly language (x86 ASM)

# Authors

Caramel has been created by KALATE Team:
- Kévin Dumanoir (kevin.dumanoir@insa-lyon.fr)
- Alexandre Gourgaud (alexandre.gourgaud@insa-lyon.fr)
- Thomas Lacroix (<thomaslacroix@free.fr>)
- Aurore Loiseau (aurore.loiseau@insa-lyon.fr)
- Elise Petit (elise.petit@insa-lyon.fr)
- Loïc Rouquette (<rouquette.loic@gmail.com>)

for the CS department - INSA Lyon 2018.


# How to download the compiler

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


# How to run the examples

**Caramel requires a C++14 compiler, at least GCC 6, and CMake 3.8.**

```shell
# Generate the Makefile using CMake
mkdir build
cd build
cmake ..

# Todo : make some examples
```
