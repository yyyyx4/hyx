
all: CFLAGS ?= -O2 -Wl,-s
all: CFLAGS += -std=c99 -pedantic -Wall -Wextra -DNDEBUG

all: *.h *.c
	$(CC) \
		$(CFLAGS) \
		hyx.c common.c blob.c history.c view.c input.c \
		-o hyx

debug: CFLAGS ?= -O0 -g
debug: CFLAGS += -std=c99 -pedantic -Wall -Wextra -Werror

debug:
	$(CC) \
		$(CFLAGS) \
		hyx.c common.c blob.c history.c view.c input.c \
		-o hyx

clean:
	rm -f hyx

