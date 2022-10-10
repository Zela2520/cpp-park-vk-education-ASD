TARGET = build/1_4

.PHONY: all rebuild build memtest clean

all: clean build 

build:
	./build.sh

memtest:
	./build.sh
	./tests/memtest.sh ./${TARGET}

clean:
	rm -rf build valgrind.log

rebuild: clean build