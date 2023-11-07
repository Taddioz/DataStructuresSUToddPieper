heapTest: lab4.o PetHeap.o
	g++ -o heapTest lab4.o PetHeap.o
lab4.o: lab4.cpp PetHeap.h
	g++ -c -Wall -Werror -pedantic -std=c++11 lab4.cpp
PetHeap.o: PetHeap.cpp PetHeap.h
	g++ -c -Wall -Werror -pedantic -std=c++11 PetHeap.cpp
