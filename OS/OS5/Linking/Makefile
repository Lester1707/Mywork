
dynamic: main.o libtree.so
	gcc -o dynamic main.o -L. -ltree -Wl,-rpath,.

main.o: main.c
	gcc -c main.c

libtree.so: tree.o
	gcc -shared -o libtree.so tree.o

tree.o: tree.c
	gcc -c -fPIC tree.c

clean:
	rm -f *.o *.a binary
