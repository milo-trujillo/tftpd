PROGNAME = tftpd

# Use user-specified compiler, or default to clang if possible, else gcc
ifeq (, $(shell which clang 2>/dev/null))
CXX?= g++
else
CXX= clang++
endif

CFLAGS += -ggdb -Wall -I/usr/local/include #-Rpass=inline
LDFLAGS += -L/usr/local/lib

OBJS = main.o write.o filesystem.o

all: $(OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $(PROGNAME) $(OBJS)

$(OBJS): %.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

test:
	@echo "Yeah, we don't have any tests."

clean:
	rm -f *.o $(PROGNAME)
