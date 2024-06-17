CC = gcc
INCLUDE = -I $(ProjectDir)\include
CFLAGS = -Wall
CFLAGS +=$(INCLUDE)
LDFLAGS = -L $(ProjectDir)lib\pdcurses.lib
LIBS = -lpdcurses

drone.exe: main.o init.o logic.o move.o my_time.o pumpkin.o 
	$(CC) -o drone main.o init.o logic.o move.o my_time.o pumpkin.o $(LIBS)

main.o: main.cpp unicode_chars.h init.h logic.h move.h my_time.h pumpkin.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o main.o main.cpp $(LIBS)

init.o: init.cpp unicode_chars.h init.h logic.h move.h my_time.h pumpkin.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o init.o init.cpp $(LIBS)

logic.o: logic.cpp unicode_chars.h init.h logic.h move.h my_time.h pumpkin.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o logic.o logic.cpp $(LIBS)

move.o: move.cpp unicode_chars.h init.h logic.h move.h my_time.h pumpkin.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o move.o move.cpp $(LIBS)

my_time.o: my_time.cpp unicode_chars.h init.h logic.h move.h my_time.h pumpkin.h 
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o my_time.o my_time.cpp $(LIBS) 

pumpkin.o: pumpkin.cpp unicode_chars.h init.h logic.h move.h my_time.h pumpkin.h 
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o pumpkin.o pumpkin.cpp $(LIBS)

clean: 
	del *.o
	del drone.exe