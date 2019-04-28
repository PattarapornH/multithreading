
FLAGS = -Wall -pthread
INCLUDES = ./include

all: simulate1 simulate2

clean:
	rm -f simulate1 *~
	rm -f simulate2 *~

simulate1: simulate1.c account.c account.h
	gcc -I $(INCLUDES) -o simulate1 simulate1.c account.c $(FLAGS)

simulate2: simulate2.c account.c account.h
	gcc -I $(INCLUDES) -o simulate2 simulate2.c account.c $(FLAGS)
