CC    = g++
AR    = ar
PLAT := linux

cFlags   := -ggdb -O0 -I.
binFlags := -ggdb -O0
libFlags := 

all: libmeshtig.a test

clean:
	rm -rf *.o test/*.o *.so ../build/* test/meshtig



libmeshtig.a: meshtigRouter.o meshtigInterface.o meshtigStack.o meshtigFrame.o protocols/proto-raw.o interfaces/int-void.o
	$(AR) rcs $@ $^
	mkdir -p ../build/include/meshtig
	mkdir -p ../build/include/meshtig/protocols
	cp ./*.hpp ../build/include/meshtig/
	cp protocols/*.hpp ../build/include/meshtig/protocols
	cp libmeshtig.a ../build/

meshtigRouter.o:
	$(CC) $(cFlags) -c meshtigRouter.cpp -o meshtigRouter.o

meshtigInterface.o:
	$(CC) $(cFlags) -c meshtigInterface.cpp -o meshtigInterface.o

meshtigStack.o:
	$(CC) $(cFlags) -c meshtigStack.cpp -o meshtigStack.o

meshtigFrame.o:
	$(CC) $(cFlags) -c meshtigFrame.cpp -o meshtigFrame.o

protocols/proto-raw.o:
	$(CC) $(cFlags) -c protocols/proto-raw.cpp -o protocols/proto-raw.o

interfaces/int-void.o:
	$(CC) $(cFlags) -c interfaces/int-void.cpp -o interfaces/int-void.o



test: test/main.o test/test-router.o test/common.o test/test-frame.o test/test-stack.o
	$(CC) $(binFlags) test/test-stack.o test/test-router.o test/common.o test/main.o test/test-frame.o -o test/meshtig -L/home/niclas/workspace/libmeshtig/src -lmeshtig -I../build/include
	cp ./test/meshtig ../build/meshtig-test
	printf "run\nwhere\nquit\n" | gdb -q ../build/meshtig-test

test/main.o:
	$(CC) $(cFlags) -c test/main.cpp -o test/main.o -I../build/include

test/test-router.o:
	$(CC) $(cFlags) -c test/test-router.cpp -o test/test-router.o -I../build/include

test/test-frame.o:
	$(CC) $(cFlags) -c test/test-frame.cpp -o test/test-frame.o -I../build/include

test/test-stack.o:
	$(CC) $(cFlags) -c test/test-stack.cpp -o test/test-stack.o -I../build/include

test/common.o:
	$(CC) $(cFlags) -c test/common.cpp -o test/common.o -I../build/include