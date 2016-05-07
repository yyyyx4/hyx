all: *.h *.c
	cc \
		-std=c99 -pedantic -Wall -Wextra -Werror \
		-D NDEBUG \
		-O2 \
			hyx.c \
			common.c \
			blob.c \
			history.c \
			view.c \
			input.c \
		-o hyx
	strip hyx

debug: *.h *.c
	cc \
		-g \
		-std=c99 -pedantic -Wall -Wextra -Werror \
			hyx.c \
			common.c \
			blob.c \
			history.c \
			view.c \
			input.c \
		-o hyx

clean:
	rm -f hyx
