cevstest: main.o test_solution_representation.o solution_representation.o read_file.o graph.o\
 		test_greedy_merge.o greedy_merge.o
	g++ -o cevstest main.o test_solution_representation.o solution_representation.o read_file.o\
		graph.o test_greedy_merge.o greedy_merge.o

main.o: main.cpp test_solution_representation.h
	g++ -c main.cpp

test_solution_representation.o: test_solution_representation.cpp test_solution_representation.h
	g++ -c test_solution_representation.cpp

test_greedy_merge.o: test_ops/test_greedy_merge.cpp test_ops/test_greedy_merge.h
	g++ -c test_ops/test_greedy_merge.cpp

solution_representation.o: ../CEVS/solution_representation.cpp ../CEVS/solution_representation.h
	g++ -c ../CEVS/solution_representation.cpp

greedy_merge.o: ../CEVS/operators/greedy_merge.h ../CEVS/operators/greedy_merge.cpp
	g++ -c ../CEVS/operators/greedy_merge.cpp

read_file.o: ../CEVS/read_file.cpp ../CEVS/read_file.h
	g++ -c ../CEVS/read_file.cpp

graph.o: ../CEVS/graph.cpp ../CEVS/graph.h
	g++ -c ../CEVS/graph.cpp

clean:
	rm *.o cevstest

