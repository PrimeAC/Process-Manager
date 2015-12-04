# Makefile 

all: commandlinereader.o par-shell.o par-shell-terminal.o list.o 
	gcc -o par-shell commandlinereader.o par-shell.o list.o -pthread -lrt
	gcc -o par-shell-terminal commandlinereader.o par-shell-terminal.o -lrt

par-shell: commandlinereader.o par-shell.o list.o 
	gcc -o par-shell commandlinereader.o par-shell.o list.o -pthread -lrt

commandlinereader.o: commandlinereader.c commandlinereader.h
	gcc -Wall -g -c commandlinereader.c

list.o: list.c list.h
	gcc -Wall -g -c list.c

par-shell.o: par-shell.c commandlinereader.h list.h
	gcc -Wall -g -c par-shell.c

par-shell-terminal.o: par-shell-terminal.c commandlinereader.h
	gcc -Wall -g -c par-shell-terminal.c

par-shell-terminal: commandlinereader.o par-shell-terminal.o
	gcc -o par-shell-terminal commandlinereader.o par-shell-terminal.o -lrt

fibonacci: fibonacci.c
	gcc -o fibonacci fibonacci.c
clear:
	rm -f *.o par-shell par-shell-terminal fibonacci log.txt *.txt 
