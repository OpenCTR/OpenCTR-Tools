========
 artool 
========

artool is a command-line tool for embedding binary resource files (including 
text files) in binaries, by converting them into linkable ARM object files.

The major distinction this tool holds over any alternatives is that it 
*directly* embeds resources into object files; meaning it just creates 
an empty ELF file with libelf, and populates the ELF sections with data from 
the input files. Compare this to solutions like bin2c and bin2s, which convert 
input files into read-only arrays in C and Assembly (respectively), and still 
have to invoke the native compiler. 

I created this because I wanted to provide something better than bin2s in 
OpenCTR.

