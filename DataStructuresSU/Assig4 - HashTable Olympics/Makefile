hashTable: assignment4.o MedalsDB.o
	g++ -o hashTable assignment4.o MedalsDB.o
assignment4.o: assignment4.cpp MedalsDB.h
	g++ -c -Wall -Werror -pedantic -std=c++11 assignment4.cpp
MedalsDB.o: MedalsDB.cpp MedalsDB.h
	g++ -c -Wall -Werror -pedantic -std=c++11 MedalsDB.cpp
