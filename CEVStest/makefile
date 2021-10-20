cevstest: main.o test_solution_representation.o solution_representation.o read_file.o graph.o
	g++ -o cevstest main.o test_solution_representation.o solution_representation.o read_file.o graph.o

main.o: main.cpp test_solution_representation.h
	g++ -c main.cpp

test_solution_representation.o: test_solution_representation.cpp test_solution_representation.h
	g++ -c test_solution_representation.cpp

solution_representation.o: ../CEVS/solution_representation.cpp ../CEVS/solution_representation.h
	g++ -c ../CEVS/solution_representation.cpp

read_file.o: ../CEVS/read_file.cpp ../CEVS/read_file.h
	g++ -c ../CEVS/read_file.cpp

graph.o: ../CEVS/graph.cpp ../CEVS/graph.h
	g++ -c ../CEVS/graph.cpp

clean:
	rm *.o cevstest

