objects = src/main.o src/system.o src/auth.o src/config.o

atm : $(objects)
	cc -o atm $(objects)

src/main.o : src/main.c src/header.h
	cc -c src/main.c -o src/main.o

src/system.o : src/system.c src/header.h
	cc -c src/system.c -o src/system.o

src/auth.o : src/auth.c src/header.h
	cc -c src/auth.c -o src/auth.o

src/config.o : src/config.c src/header.h
	cc -c src/config.c -o src/config.o

clean :
	rm -f $(objects) atm