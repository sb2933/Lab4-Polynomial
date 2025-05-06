CC = g++

CFLAGS = -Wall

OBJECTS = main.o Polynomial.o \
		Term.o Variable.o 

Project2 :  $(OBJECTS)
	$(CC) $(CFLAGS) -o Project2 $(OBJECTS)

main.o : main.cpp Polynomial.h Term.h Variable.h 
	$(CC) $(CFLAGS) -c main.cpp


Polynomial.o : Polynomial.h Term.h Variable.h 

Term.o : Term.h Variable.h 


Variable.o : Variable.h 

.PHONY : clean
clean :
	rm Project2 $(OBJECTS)