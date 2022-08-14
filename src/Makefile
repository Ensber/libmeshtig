CC    = g++
AR    = ar
PLAT := linux

cFlags   := -ggdb -O0
binFlags := -ggdb -O0
libFlags := 

all: libmeshtig.a test

clean:
	rm -rf *.o test/*.o *.so ../build/* test/meshtig



libmeshtig.a: meshtigRouter.o meshtigInterface.o meshtigStack.o meshtigFrame.o
	$(AR) rcs $@ $^
	mkdir -p ../build/include/meshtig
	cp *.hpp ../build/include/meshtig/
	cp libmeshtig.a ../build/

meshtigRouter.o:
	$(CC) $(cFlags) -c meshtigRouter.cpp -o meshtigRouter.o

meshtigInterface.o:
	$(CC) $(cFlags) -c meshtigInterface.cpp -o meshtigInterface.o

meshtigStack.o:
	$(CC) $(cFlags) -c meshtigStack.cpp -o meshtigStack.o

meshtigFrame.o:
	$(CC) $(cFlags) -c meshtigFrame.cpp -o meshtigFrame.o



test: test/main.o test/test-router.o test/common.o test/test-frame.o
	$(CC) $(binFlags) test/test-router.o test/common.o test/main.o test/test-frame.o -o test/meshtig -L/home/niclas/workspace/libmeshtig/src -lmeshtig -I../build/include
	cp ./test/meshtig ../build/meshtig-test
	printf "run\nwhere\nquit\n" | gdb ../build/meshtig-test

test/main.o:
	$(CC) $(cFlags) -c test/main.cpp -o test/main.o -I../build/include

test/test-router.o:
	$(CC) $(cFlags) -c test/test-router.cpp -o test/test-router.o -I../build/include

test/test-frame.o:
	$(CC) $(cFlags) -c test/test-frame.cpp -o test/test-frame.o -I../build/include

test/common.o:
	$(CC) $(cFlags) -c test/common.cpp -o test/common.o -I../build/include