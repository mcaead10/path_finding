# Makefile pour INF3105 / TP3

#OPTIONS = -Wall           # option standard
#OPTIONS = -g -O0 -Wall    # pour rouler dans gdb
OPTIONS = -O2 -Wall        # pour optimiser

#
all : tp3a tp3b

tp3a : tp3a.cpp carte.h carte.o coordonnee.h coordonnee.o heure.o 
	g++ ${OPTIONS} -o tp3a tp3a.cpp coordonnee.o carte.o heure.o 

tp3b : tp3b.cpp carte.o coordonnee.o heure.o reseau.o
	g++ ${OPTIONS} -o tp3b tp3b.cpp carte.o coordonnee.o heure.o reseau.o

carte.o : carte.cpp carte.h coordonnee.h
	g++ ${OPTIONS} -c -o carte.o carte.cpp

coordonnee.o : coordonnee.cpp coordonnee.h
	g++ ${OPTIONS} -c -o coordonnee.o coordonnee.cpp

heure.o : heure.cpp heure.h
	g++ ${OPTIONS} -c -o heure.o heure.cpp

reseau.o : reseau.cpp reseau.h coordonnee.h
	g++ ${OPTIONS} -c -o reseau.o reseau.cpp

clean:
	rm -rf tp3[ab] *~ *.o

