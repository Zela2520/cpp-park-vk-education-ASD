TARGET = build/1_6

.PHONY: all rebuild build memtest clean

all: clean build run

build:
	./build.sh

run: build
	./${TARGET}

memtest:
	./build.sh
	./tests/memtest.sh ./${TARGET}

clean:
	rm -rf build valgrind.log

rebuild: clean build