CC=gcc

compile:
	$(CC) -Wall -o tabby main.c

run: compile
	./tabby

clean:
	rm tabby
