CC=gcc

compile: clean
	$(CC) -Wall -Werror -o tabby main.c id3v2.c util.c

run: compile
	./tabby data/soviet_anthem.mp3

clean:
	rm tabby || true
