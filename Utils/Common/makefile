all: common.c.o
	ar rcs libcommon.a common.c.o

common.c.o: common.c common.h
	gcc -c -fPIC -I./ common.c -o common.c.o

clean:
	rm *.o
	rm *.a