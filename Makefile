
all  : aventuriers

aventuriers : aventurier.c
	gcc -Wall -o aventurier.c aventuriers

clean :
	rm -f *.o
	rm -f aventuriers

go : clean all
	./aventuriers

debug : 
	gcc -Wall -g -o aventuriers aventurier.c