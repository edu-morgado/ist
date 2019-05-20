CircuitRouter-SimpleShell: CircuitRouter-SimpleShell.o ../lib/commandlinereader.o
	gcc CircuitRouter-SimpleShell.o ../lib/commandlinereader.o -lm -o CircuitRouter-SimpleShell

CircuitRouter-SimpleShell.o: CircuitRouter-SimpleShell.c ../lib/commandlinereader.h ../lib/commandlinereader.c
	gcc -Wall -std=gnu99 -c CircuitRouter-SimpleShell.c -o CircuitRouter-SimpleShell.o

clean:
	rm CircuitRouter-SimpleShell *.o
