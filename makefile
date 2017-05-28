resalt: resalt.c
	gcc -Wall -O3 -Wextra -pedantic -o resalt resalt.c

clean:
	rm -f resalt
