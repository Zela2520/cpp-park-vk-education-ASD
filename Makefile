TARGET = build/1_3

.PHONY: all build rebuild memtest clean

all: clean build memtest

build:
	./build.sh

rebuild: clean build

memtest:
	./build.sh
	./tests/memtest.sh ./${TARGET}

clean:
	rm -rf build valgrind.log