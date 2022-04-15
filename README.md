# Hexdiff

```
Warning: Do not run on large files.
```

Get the diff of binary files.

Implemented by reversing the solution to the Greatest-Common-Subsequence problem for two byte streams.

Based on the work by Curtis Li.

## Usage

```
./hexdiff FILE1 FILE2
```

## Installation

Hexdiff requires argp to build. You can get argp on Mac OS by doing:
```
brew install argp-standalone
```

To build the system, run:

```
git clone https://github.com/jtpastro/hexdiff.git
cd hexdiff && make
```

## Features

* Get the diff of binary files
* Prints the differences in coloured text

