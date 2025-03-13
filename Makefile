


all  : aventurier.o ../TicketToRideAPI/tickettorideapi/codingGameServer.o ../TicketToRideAPI/tickettorideapi/ticketToRide.o
	gcc -o dab aventurier.o ../TicketToRideAPI/tickettorideapi/codingGameServer.o ../TicketToRideAPI/tickettorideapi/ticketToRide.o

aventurier.o : aventurier.c
	gcc -c -o aventurier.o aventurier.c

../TicketToRideAPI/tickettorideapi/codingGameServer.o : ../TicketToRideAPI/tickettorideapi/codingGameServer.c ../TicketToRideAPI/tickettorideapi/codingGameServer.h
	gcc -c -o ../TicketToRideAPI/tickettorideapi/codingGameServer.o ../TicketToRideAPI/tickettorideapi/codingGameServer.c

../TicketToRideAPI/tickettorideapi/ticketToRide.o : ../TicketToRideAPI/tickettorideapi/ticketToRide.c ../TicketToRideAPI/tickettorideapi/ticketToRide.h
	gcc -c -o ../TicketToRideAPI/tickettorideapi/ticketToRide.o ../TicketToRideAPI/tickettorideapi/ticketToRide.c

clean :
	rm -f *.o
	rm -f ../TicketToRideAPI/tickettorideapi/*.o
	rm -f dab

go : clean all
	./dab

debug : clean
	gcc -c -g -o ticketToRide.o ticketToRide.c
	gcc -c -g -o codingGameServer.o codingGameServer.c
	gcc -c -g -o aventurier.o aventurier.c
	gcc -g -o dab aventurier.o codingGameServer.o ticketToRide.o
