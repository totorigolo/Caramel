Caramel - C Compiler with syntactic sugar
========================

Caramel is a compiler of a limited subset of the C language. Caramel is divided into 2 parts:
- The front-end, which creates an Abstract Syntax Tree from the source code.
- The back-end, which compile the AST into full assembly language (x86_64 ASM).

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

```shell
# Git clone with SSH
git clone --recursive git@gitlab.com:hexanome-kalate/pld-comp.git

# Git clone with HTTPS
git clone --recursive https://gitlab.com/hexanome-kalate/pld-comp.git

# If already cloned
git submodule update --init --recursive
```


## How to run the examples

**Caramel requires a C++17 compiler (at least GCC 6) and CMake 3.8.**

### Chef commands :

Caramel uses Chef, it's cooking assistant, for almost every operations.

```bash
$ ./chef.py -h
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

Some commands:

```bash
# Build the Antlr parser and Caramel. This is rarely needed as `test all -b` does the same, and more.
./chef.py build -a

# Clean the project
./chef.py clean
```


### Example for executing all test
```bash
$ ./chef.py test -h
usage: chef.py test [-h] {grammar,semantic,backend,programs,all} ...

optional arguments:
  -h, --help            show this help message and exit

Available sub-commands:
  {grammar,semantic,backend,programs,all}
    grammar             Test the Caramel grammar.
    semantic            Test the Caramel semantic analysis.
    backend             Test the Caramel back-end.
    programs            Test the execution of some example programs.
    all                 Run all tests.

$ ./chef.py test all -b
```
Will build the project and execute all the tests.

* Tests marked as `[+]` are valid tests that must succeed.
* Tests marked as `[-]` are invalid tests that must fail.

**Note:** Some tests require inputs, such as `get_char.c` and `interactive_factorial.c`.
For these two, just enter a digit then press Enter.


## Caramel usage

We encourage you to run `./build/cpp-bin/Caramel --help` to see every
Caramel options. Here is the most common usage:

```bash
# Compile (-c), assemble (-A) and execute (-A again), with static analysis (-a)
# the source file path/to/source/file.c. -vv increase the verbosity.
# --ast-dot generates the ast.pdf file, and --ir-dot the ir.pdf file.

cd ./build/cpp-bin
./Caramel -vv -acA --ast-dot --ir-dot path/to/source/file.c

# You can open ast.pdf and ir.pdf
xdg-open ast.pdf
xdg-open ir.pdf
```

**Note:** You may encounter an error with a shared library, such as:
```
./Caramel: error while loading shared libraries:
libantlr4-runtime.so.4.7.1: cannot open shared object file: No such file or directory
```
You just have to prepend `LD_LIBRARY_PATH=lib`:
```bash
LD_LIBRARY_PATH=../../lib ./Caramel ...
```

## Grammar
- [Grammar](doc/grammar.md)

While writing the grammar, we used Chef to help us: by running grammar
tests, and by showing us the syntax tree. Chef has a lot of useful
options, so we won't show them all. Please refer to the help through
the `--help` flag.

```bash
# Build the Antlr parser and run all the grammar tests, showing the syntax tree for failed tests
./chef.py test grammar -baG

# Build the Antlr parser and run a single test with the syntax tree
./chef.py test grammar -bg path/to/a/test.c

# Check a source interactively
./chef.py test grammar -i
```

## Semantic
- [Semantic](doc/semantic.md)

As for the grammar, we used Chef for executing the tests:

```bash
# Run semantic tests
./chef.py test semantic -ba
```

But we also developped a PDF export of the AST, to have a better insight
of our work. It's available as a command line argument for Caramel:

```bash
# Generate the ast.pdf file
cd ./build/cpp-bin
./Caramel --ast-dot
```

## Back-end phases
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


As for the semantic phase, we used Chef and Caramel:

```bash
# Run back-end tests
./chef.py test semantic -ba

# Generate the ir.pdf file
cd ./build/cpp-bin
./Caramel -c --ir-dot
```


## What's working (not exhaustive)?
- [x] variable definition
- [x] function definition
- [x] array definition
- [x] more than 6 args function call
- [x] recursive function call
- [x] operators priority
- [x] complex nested expression (ex: `foo(1 || 4 & 2) || (1>2, 2<4, foo(97 & 8 || 7)) || (1==1 && 4>3)`)
- [x] if ... then ... else
- [x] while block
- [x] do while block
- [x] for block
- [x] real stack size (in prolog)
- [x] break / return
- [ ] correct types handling (int8_t, int16_t -> int32_t)
