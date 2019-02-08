CC=gcc
VALGRIND=valgrind

compile: clean
	$(CC) -Wall -Werror -o tabby main.c id3v2.c util.c

run: compile
	./tabby data/soviet_anthem.mp3

clean:
	rm tabby || true

memcheck: clean
	which $(VALGRIND)	# will fail if valgrind not installed
	$(CC) -Wall -Werror -g -O0 -o tabby main.c id3v2.c util.c
	$(VALGRIND) --leak-check=yes ./tabby data/soviet_anthem.mp3
