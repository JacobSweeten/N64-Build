includes = -I./ -I../Common

all: PackAssets.c.o
	gcc PackAssets.c.o -L../Common -lcommon -o packassets

PackAssets.c.o: PackAssets.c PackAssets.h
	gcc $(includes) -c PackAssets.c -o PackAssets.c.o

clean:
	rm packassets
	rm *.o