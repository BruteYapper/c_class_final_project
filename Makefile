OBJ = main.o connect.o escape.o flyer.o hollow.o hero.o spectator.o
EXE = a5
CC = gcc -Wall -g

$(EXE):	$(OBJ)
	$(CC) -o $(EXE) $(OBJ)

main.o:	main.c defs.h
	$(CC) -c main.c

connect.o:	connect.c defs.h
	$(CC) -c connect.c

escape.o: escape.c defs.h 
	$(CC) -c escape.c

flyer.o: flyer.c defs.h 
	$(CC) -c flyer.c 

hollow.o: hollow.c defs.h 
	$(CC) -c hollow.c 

hero.o: hero.c defs.h 
	$(CC) -c hero.c 
spectator.o: spectator.c defs.h
	$(CC) -c spectator.c

clean:
	rm -f $(EXE) $(OBJ)

