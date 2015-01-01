TFAT
====
#Introduction
TOs kernel FAT16 & FAT32 filesystem parser and handler with interactive shell  

This project originally written for assembly programming course at Amirkabir university. This project completely written by C under linux mint with gcc.  
Following library from GNU with GPL used in this project:  
1) GNU readline  
2) GNU history  

In this project we using kernel-coding style from Linus Torvalds and using some script from kernel to validate our code against that.

Some TFAT features include:
1. FAT type detection  
2. Support FAT16 & FAT32  
3. Delete & Undelete  
4. ls & cd & dumo ... commands  
&...

#Contribution
For todo list see doc/TODlist.md  
If you have bug or idea about this project send me an [email](mailto:parham.alvani@gmail.com)

#Dependencies
1. GNU readline
2. GNU histroy  
both project available at [GNU READLINE HOMEPAGE](http://cnswww.cns.cwru.edu/php/chet/readline/rltop.html)

#Installation
After compiling GNU readline copy libreadline.a & libhistory.a into src folder and execute following command:

	$ make


