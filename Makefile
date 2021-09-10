GCC=gcc
GCC_FLAGS= -g -std=c99 -Wall
O_FLAGS= -lrt -lpthread

all: App Slave View

App: src/app.c src/library.c
	$(GCC) $(GCC_FLAGS) -I./include src/app.c src/library.c -o App $(O_FLAGS)

Slave: src/slave.c src/library.c
	$(GCC) $(GCC_FLAGS) -I./include src/slave.c src/library.c -o Slave $(O_FLAGS)

View: src/view.c src/library.c
	$(GCC) $(GCC_FLAGS) -I./include src/view.c src/library.c -o View $(O_FLAGS)

clean:
	rm -rf App
	rm -rf Slave
	rm -rf View

.PHONY: clean all
