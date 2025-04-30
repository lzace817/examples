include .config

CFLAGS = -Wall -Wextra -ggdb

hello: main.c .config
	${CC} ${CFLAGS} $< -o $@ -D ${MSGT}

.PHONY: clean

clean:
	rm -f hello