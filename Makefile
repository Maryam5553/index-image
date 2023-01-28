GCC = gcc --std=gnu17 -Wall

# -help qui affiche les arguments à disposition
# -sat -bw -gray -444

all: index

index: main.o pngio.o linkedList.o node.o palette.o
	$(GCC) -o index main.o pngio.o linkedList.o node.o palette.o -lpng

main.o: main.c pngio.h node.h palette.h
	$(GCC) -c main.c

palette.o: palette.c palette.h
	$(GCC) -c palette.c

node.o: node.c node.h linkedList.h pngio.h
	$(GCC) -c node.c

linkedList.o: linkedList.c linkedList.h
	$(GCC) -c linkedList.c

pngio.o: pngio.c pngio.h
	$(GCC) -c pngio.c

clean:
	rm *.o