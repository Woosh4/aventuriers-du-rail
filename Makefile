
FLAG_DEBUG= -g

all  : aventurier.o ../TicketToRideAPI/tickettorideapi/codingGameServer.o ../TicketToRideAPI/tickettorideapi/ticketToRide.o
	gcc $(FLAG_DEBUG) -o dab aventurier.o ../TicketToRideAPI/tickettorideapi/codingGameServer.o ../TicketToRideAPI/tickettorideapi/ticketToRide.o

aventurier.o : aventurier.c aventurier.h
	gcc -c $(FLAG_DEBUG) -o aventurier.o aventurier.c

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
