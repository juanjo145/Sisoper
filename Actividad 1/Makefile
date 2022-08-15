CC=gcc

%.o: %.c
	$(CC) -c -o $@ $<

demo3: demo.o
	gcc -o demo3 demo.o


tar:
	tar czvf c_language_basics.tgz *.c 
	
clean:
	rm -f *.o demo3
