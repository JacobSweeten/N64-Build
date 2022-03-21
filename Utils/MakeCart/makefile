all: MakeCart.c.o
	gcc MakeCart.c.o -L../Common -lcommon -o makecart

MakeCart.c.o: MakeCart.c
	gcc -c -I../Common MakeCart.c -o MakeCart.c.o

clean:
	rm makecart
	rm *.o