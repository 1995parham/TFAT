# TFAT
## Introduction
TOs kernel FAT16 & FAT32 filesystem parser and handler with interactive shell  

This project was originally written for assembly programming course at
[Amirkabir university](http://www.aut.ac.ir).
This project was completely written by C under ubuntu with gcc.  

In this project we use kernel-coding style from Linus Torvalds and
use some script from kernel to validate our code against that.

Some TFAT features include:

1. FAT type detection
2. Support FAT16 & FAT32
3. Delete & Un-delete
4. ls & cd & dump ... commands  
&...

## Contribution
For todo list see doc/TODlist.md  
If you have bug or idea about this project send us a pull request.

## Contributors
* [Parham Alvani](https://1995parham.github.io)
* [Elahe Jalalpour](http://ceit.aut.ac.ir/~jalalpour)

## Dependencies
1. GNU readline
2. GNU history  
both project available at
[GNU READLINE HOMEPAGE](http://cnswww.cns.cwru.edu/php/chet/readline/rltop.html)

## Installation
```
$ cmake .
$ make
```