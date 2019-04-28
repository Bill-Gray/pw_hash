all: resalt pw

resalt: resalt.c
	gcc -Wall -O3 -Wextra -pedantic -o resalt resalt.c

pw: pw.c
	gcc -Wall -O3 -Wextra -pedantic -o pw pw.c sha256.c

clean:
	rm -f resalt
	rm -f pw
