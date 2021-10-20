cevs: main.o read_file.o graph.o
	g++ -o cevs main.o read_file.o

main.o: main.cpp read_file.h
	g++ -c main.cpp

read_file.o: read_file.cpp read_file.h
	g++ -c read_file.cpp

graph.o: graph.cpp graph.h
	g++ -c graph.cpp

clean:
	rm *.o cevs
