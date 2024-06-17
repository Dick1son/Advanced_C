CC = gcc
CFLAGS = -Wall

all: drone.exe

drone.exe: main.o init.o logic.o move.o my_time.o pumpkin.o 
	$(CC) -o drone main.o init.o logic.o move.o my_time.o pumpkin.o C:\PDCurses-3.8\wincon\pdcurses.a -I C:\PDCurses-3.8 -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x86" -lUser32

main.o: main.cpp unicode_chars.h init.h logic.h move.h my_time.h pumpkin.h
	$(CC) $(CFLAGS) -c main.cpp C:\PDCurses-3.8\wincon\pdcurses.a -I C:\PDCurses-3.8 -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x86" -lUser32

init.o: init.cpp unicode_chars.h init.h logic.h move.h my_time.h pumpkin.h  
	$(CC) $(CFLAGS) -c init.cpp C:\PDCurses-3.8\wincon\pdcurses.a -I C:\PDCurses-3.8 -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x86" -lUser32

logic.o: logic.cpp unicode_chars.h init.h logic.h move.h my_time.h pumpkin.h
	$(CC) $(CFLAGS) -c logic.cpp C:\PDCurses-3.8\wincon\pdcurses.a -I C:\PDCurses-3.8 -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x86" -lUser32

move.o: move.cpp unicode_chars.h init.h logic.h move.h my_time.h pumpkin.h 
	$(CC) $(CFLAGS) -c move.cpp C:\PDCurses-3.8\wincon\pdcurses.a -I C:\PDCurses-3.8 -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x86" -lUser32

my_time.o: my_time.cpp unicode_chars.h init.h logic.h move.h my_time.h pumpkin.h  
	$(CC) $(CFLAGS) -c my_time.cpp C:\PDCurses-3.8\wincon\pdcurses.a -I C:\PDCurses-3.8 -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x86" -lUser32

pumpkin.o: pumpkin.cpp unicode_chars.h init.h logic.h move.h my_time.h pumpkin.h  
	$(CC) $(CFLAGS) -c pumpkin.cpp C:\PDCurses-3.8\wincon\pdcurses.a -I C:\PDCurses-3.8 -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x86" -lUser32

clean:
	del *.o
	del drone.exe