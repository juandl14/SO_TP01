GCC=gcc
GCC_FLAGS= -g -std=c99 -Wall
O_FLAGS= -lrt -lpthread

all: App Slave # View

App: src/app.c src/errors.c include/app.h include/errors.h
	$(GCC) $(GCC_FLAGS) -I./include src/app.c src/errors.c -o App $(O_FLAGS)

Slave:
	$(GCC) $(GCC_FLAGS) -I./include src/slave.c src/errors.c -o Slave $(O_FLAGS)
clean:
	rm -rf App
	rm -rf Slave

.PHONY: clean all
