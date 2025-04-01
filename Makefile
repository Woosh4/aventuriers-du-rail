
FLAG_DEBUG= -g

all  : aventurier.o bot1.o gestion.o manual_play.o ../TicketToRideAPI/tickettorideapi/codingGameServer.o ../TicketToRideAPI/tickettorideapi/ticketToRide.o
	gcc $(FLAG_DEBUG) -o dab $^

aventurier.o : aventurier.c aventurier.h
	gcc -c $(FLAG_DEBUG) -o aventurier.o aventurier.c

gestion.o : gestion.c gestion.h
	gcc -c $(FLAG_DEBUG) -o gestion.o gestion.c

manual_play.o : manual_play.c manual_play.h
	gcc -c $(FLAG_DEBUG) -o manual_play.o manual_play.c

bot1.o : bot1.c bot1.h
	gcc -c $(FLAG_DEBUG) -o bot1.o bot1.c

../TicketToRideAPI/tickettorideapi/codingGameServer.o : ../TicketToRideAPI/tickettorideapi/codingGameServer.c ../TicketToRideAPI/tickettorideapi/codingGameServer.h
	gcc -c $(FLAG_DEBUG) -o ../TicketToRideAPI/tickettorideapi/codingGameServer.o ../TicketToRideAPI/tickettorideapi/codingGameServer.c

../TicketToRideAPI/tickettorideapi/ticketToRide.o : ../TicketToRideAPI/tickettorideapi/ticketToRide.c ../TicketToRideAPI/tickettorideapi/ticketToRide.h
	gcc -c $(FLAG_DEBUG) -o ../TicketToRideAPI/tickettorideapi/ticketToRide.o ../TicketToRideAPI/tickettorideapi/ticketToRide.c

clean :
	rm -f *.o
	rm -f ../TicketToRideAPI/tickettorideapi/*.o
	rm -f dab

go : clean all
	./dab
