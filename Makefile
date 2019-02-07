CC=gcc

compile: clean
	$(CC) -Wall -Werror -o tabby main.c

run: compile
	./tabby data/soviet_anthem.mp3

clean:
	rm tabby || true
