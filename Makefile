# Makefile 

par-shell: par-shell.o commandlinereader.o
	gcc -o par-shell par-shell.o commandlinereader.o

commandlinereader.o: commandlinereader.c commandlinereader.h
	gcc -Wall -g -c commandlinereader.c

par-shell.o: par-shell.c commandlinereader.h
	gcc -Wall -g -c par-shell.c commandlinereader.c
clear:
	rm -f *.o par-shell
