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
If you have an idea or find a bug feel free to fork TFAT and
send us a pull request and you can see [here](doc/TODOlist.md)
for TFAT TODO list.

## Contributors
* [Parham Alvani](https://1995parham.github.io)

## Dependencies
1. GNU readline
2. GNU history  
both project available at
[GNU READLINE HOMEPAGE](http://cnswww.cns.cwru.edu/php/chet/readline/rltop.html)

## Installation
```shell
mkdir build; cd build
cmake ../
make
```

## References
For FAT structure details and our references implementation
please see following links:

- [A tutorial on the FAT file system](http://www.tavi.co.uk/phobos/fat.html)
- [FAT](http://wiki.osdev.org/FAT)
- [Design of the FAT file system](http://en.wikipedia.org/wiki/Design_of_the_FAT_file_system)
- [FAT Utilities](https://gitorious.org/unix-stuff/fat-util)
- [Grasso - a FAT filesystem parser written in Python](https://github.com/em-/grasso)
- [Un-Deletion](http://en.wikipedia.org/wiki/Undeletion)
