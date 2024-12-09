command: main.c environment.c command.c direct.c
	gcc -o command main.c environment.c command.c direct.c

clean:
	rm -f command main.o

	
