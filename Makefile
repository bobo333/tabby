CC=gcc

compile: clean
	$(CC) -Wall -Werror -o tabby main.c

run: compile
	./tabby

clean:
	rm tabby || true
