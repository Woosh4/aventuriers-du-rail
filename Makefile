
FLAG_DEBUG= -g

all  : aventurier.o bot1.o bot2.o bot3.o gestion.o manual_play.o ../TicketToRideAPI/tickettorideapi/clientAPI.o ../TicketToRideAPI/tickettorideapi/ticketToRide.o
	gcc $(FLAG_DEBUG) -o dab $^

aventurier.o : aventurier.c aventurier.h
	gcc -c $(FLAG_DEBUG) -o aventurier.o aventurier.c

gestion.o : gestion.c gestion.h
	gcc -c $(FLAG_DEBUG) -o gestion.o gestion.c

manual_play.o : manual_play.c manual_play.h
	gcc -c $(FLAG_DEBUG) -o manual_play.o manual_play.c

bot1.o : bot1.c bot1.h
	gcc -c $(FLAG_DEBUG) -o bot1.o bot1.c

bot2.o : bot2.c bot2.h
	gcc -c $(FLAG_DEBUG) -o bot2.o bot2.c

bot3.o : bot3.c bot3.h
	gcc -c $(FLAG_DEBUG) -o bot3.o bot3.c

../TicketToRideAPI/tickettorideapi/clientAPI.o : ../TicketToRideAPI/tickettorideapi/clientAPI.c ../TicketToRideAPI/tickettorideapi/clientAPI.h
	gcc -c $(FLAG_DEBUG) -o ../TicketToRideAPI/tickettorideapi/clientAPI.o ../TicketToRideAPI/tickettorideapi/clientAPI.c

../TicketToRideAPI/tickettorideapi/ticketToRide.o : ../TicketToRideAPI/tickettorideapi/ticketToRide.c ../TicketToRideAPI/tickettorideapi/ticketToRide.h
	gcc -c $(FLAG_DEBUG) -o ../TicketToRideAPI/tickettorideapi/ticketToRide.o ../TicketToRideAPI/tickettorideapi/ticketToRide.c

clean :
	rm -f *.o
	rm -f ../TicketToRideAPI/tickettorideapi/*.o
	rm -f dab

go : clean all
	./dab
