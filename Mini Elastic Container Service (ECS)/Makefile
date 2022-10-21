CC=gcc

%.o: %.c
	$(CC) -c -o $@ $<

cliente: cliente.o
	gcc -o cliente cliente.o


servidor: servidor.o
	gcc -o servidor servidor.o


tar:
	tar czvf c_language_basics.tgz *.c 
	
clean:
	rm -f *.o servidor
	rm -f *.o cliente 
	