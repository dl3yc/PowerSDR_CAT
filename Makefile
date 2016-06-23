CC = i686-w64-mingw32-gcc
LD = i686-w64-mingw32-gcc
CFLAGS = -g -Wall -I/usr/local/include
LDFLAGS = -lserialport -L/usr/local/lib
RM = /bin/rm -f
OBJS = main.o knwd.o
PROG = sdrctrl.exe

all: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(PROG) 

main.o: main.c knwd.c
	$(CC) $(CFLAGS) -c main.c

knwd.o: knwd.c knwd.h
	$(CC) $(CFLAGS) -c knwd.c

clean:
	$(RM) $(PROG) $(OBJS)
