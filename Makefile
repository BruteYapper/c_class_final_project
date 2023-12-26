OBJ = main.o connect.o escape.o flyer.o hollow.o hero.o spectator.o
EXE = a5
CC = gcc -Wall -g
LIBS = -lpthread

$(EXE):	$(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(LIBS)

main.o:	main.c defs.h
	$(CC) -c main.c $(LIBS)

connect.o:	connect.c defs.h
	$(CC) -c connect.c $(LIBS)

escape.o: escape.c defs.h 
	$(CC) -c escape.c $(LIBS)

flyer.o: flyer.c defs.h 
	$(CC) -c flyer.c $(LIBS)

hollow.o: hollow.c defs.h 
	$(CC) -c hollow.c $(LIBS)

hero.o: hero.c defs.h 
	$(CC) -c hero.c $(LIBS)
spectator.o: spectator.c defs.h
	$(CC) -c spectator.c $(LIBS)

clean:
	rm -f $(EXE) $(OBJ)

