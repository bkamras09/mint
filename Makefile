k: src/main.c
	gcc -o k src/main.c -lm -Wall -Wextra -Werror -pedantic -std=c99

clean:
	rm -f k

.PHONY: clean