#+----------------------------------------------------+
#|                                                    |
#|        #######                                     |
#|           #    #  ####  #    # ###### #####        |
#|           #    # #    # #   #  #        #          |
#|           #    # #      ####   #####    #          |
#|           #    # #      #  #   #        #          |
#|           #    # #    # #   #  #        #          |
#|           #    #  ####  #    # ######   #          |
#|                                                    |
#|                      ######                        |
#|      #####  ####     #     # # #####  ######       |
#|        #   #    #    #     # # #    # #            |
#|        #   #    #    ######  # #    # #####        |
#|        #   #    #    #   #   # #    # #            |
#|        #   #    #    #    #  # #    # #            |
#|        #    ####     #     # # #####  ######       |
#|                                                    |
#|                                                    |
#+----------------------------------------------------+

# options de compilation
CC = gcc
CCFLAGS = -Wall
LIBS = 
LIBSDIR = 

# fichiers du projet
SRC = jeu.c move.c clientAPI.c TicketToRideAPI.c
OBJ = $(SRC:.c=.o)
EXEC = jeu.out


# règle initiale
all: $(EXEC)

# dépendance des .h
clientAPI.o: clientAPI.h
TicketToRideAPI.o: clientAPI.h TicketToRideAPI.h
jeu.o: move.h TicketToRideAPI.h

# règles de compilation
%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<
	
# règles d'édition de liens
$(EXEC): $(OBJ)
	$(CC) $(CCFLAGS) -o $@ $^ $(LIBS) $(LIBSDIR)

# règles supplémentaires
clean:
	rm -f *.o
mproper:
	rm -f $(EXEC) *.o