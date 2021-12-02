cevstest: main.o test_solution_representation.o solution_representation.o read_file.o graph.o\
 		test_greedy_merge.o greedy_merge.o read_sol.o test_add_node.o add_node.o weighted_random.o
	g++ -o cevstest main.o test_solution_representation.o solution_representation.o read_file.o\
		graph.o test_greedy_merge.o greedy_merge.o read_sol.o test_add_node.o add_node.o weighted_random.o

main.o: main.cpp test_solution_representation.h
	g++ -c main.cpp

test_solution_representation.o: test_solution_representation.cpp test_solution_representation.h
	g++ -c test_solution_representation.cpp

test_greedy_merge.o: test_ops/test_greedy_merge.cpp test_ops/test_greedy_merge.h
	g++ -c test_ops/test_greedy_merge.cpp

test_add_node.o: test_ops/test_add_node.cpp test_ops/test_add_node.h
	g++ -c test_ops/test_add_node.cpp

solution_representation.o: ../CEVS/solution_representation.cpp ../CEVS/solution_representation.h
	g++ -c ../CEVS/solution_representation.cpp

greedy_merge.o: ../CEVS/operators/greedy_merge.h ../CEVS/operators/greedy_merge.cpp
	g++ -c ../CEVS/operators/greedy_merge.cpp

add_node.o: ../CEVS/operators/add_node.h ../CEVS/operators/add_node.cpp
	g++ -c ../CEVS/operators/add_node.cpp


read_file.o: ../CEVS/read_file.cpp ../CEVS/read_file.h
	g++ -c ../CEVS/read_file.cpp

graph.o: ../CEVS/graph.cpp ../CEVS/graph.h
	g++ -c ../CEVS/graph.cpp

read_sol.o: read_sol.cpp read_sol.h
	g++ -c read_sol.cpp

weighted_random.o: ../CEVS/utility/weighted_random.cpp ../CEVS/utility/weighted_random.h
	g++ -c ../CEVS/utility/weighted_random.cpp

clean:
	rm *.o cevstest

