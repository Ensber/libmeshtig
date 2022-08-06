CC    = g++
AR    = ar
PLAT := linux

cFlags   := -ggdb -O0
binFlags := -ggdb -O0
libFlags := 

all: libmeshtig.a test

clean:
	rm -rf *.o test/*.o *.so ../build/* test/meshtig


libmeshtig.a: meshtigRouter.o
	$(AR) rcs $@ $^
	mkdir -p ../build/include/meshtig
	cp *.hpp ../build/include/meshtig/
	cp libmeshtig.a ../build/

meshtigRouter.o:
	$(CC) $(cFlags) -c meshtigRouter.cpp -o meshtigRouter.o


test: test/main.o
	$(CC) $(binFlags) test/main.cpp -o test/meshtig -L/home/niclas/workspace/libmeshtig/src -lmeshtig -I../build/include
	cp ./test/meshtig ../build/meshtig-test
	../build/meshtig-test

test/main.o:
	$(CC) $(cFlags) -c test/main.cpp -o test/main.o -I../build/include