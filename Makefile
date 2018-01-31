PROGNAME = tftpd

# Use user-specified compiler, or default to clang if possible, else gcc
ifeq (, $(shell which clang 2>/dev/null))
CC?= gcc
else
CC?= clang
endif

CFLAGS += -ggdb -Wall -I/usr/local/include #-Rpass=inline
LDFLAGS += -L/usr/local/lib

OBJS = main.o 

all: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROGNAME) $(OBJS)

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test:
	@echo "Yeah, we don't have any tests."

clean:
	rm -f *.o $(PROGNAME)
