
all  : aventuriers

aventuriers : aventurier.c
	gcc -Wall -o aventurier.c aventuriers

clean :
	rm -f aventurier.o
	rm -f aventuriers

go : clean aventuriers
	./aventuriers