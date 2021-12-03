
cevs: main.o read_file.o graph.o solution_representation.o add_node.o \
		greedy_merge.o local_search.o weighted_random.o bookkeep.o \
		b_merge.o shallow_solution.o
	g++ -g -o cevs main.o read_file.o graph.o solution_representation.o add_node.o \
		greedy_merge.o local_search.o weighted_random.o bookkeep.o b_merge.o \
		shallow_solution.o

main.o: main.cpp read_file.h
	g++ -c -g main.cpp

read_file.o: read_file.cpp read_file.h
	g++ -c -g read_file.cpp

graph.o: graph.cpp graph.h
	g++ -c -g graph.cpp

solution_representation.o: solution_representation.cpp solution_representation.h
	g++ -c -g solution_representation.cpp

add_node.o: operators/add_node.cpp operators/add_node.h
	g++ -c -g operators/add_node.cpp

greedy_merge.o: operators/greedy_merge.cpp operators/greedy_merge.h
	g++ -c -g operators/greedy_merge.cpp

local_search.o: metaheuristics/local_search.cpp metaheuristics/local_search.h
	g++ -c -g metaheuristics/local_search.cpp

weighted_random.o: utility/weighted_random.cpp utility/weighted_random.h
	g++ -c -g utility/weighted_random.cpp

bookkeep.o: bookkeep/bookkeep.cpp bookkeep/bookkeep.h
	g++ -c -g bookkeep/bookkeep.cpp

b_merge.o: bookkeep/b_merge.cpp bookkeep/b_merge.h
	g++ -c -g bookkeep/b_merge.cpp

shallow_solution.o: utility/shallow_solution.cpp utility/shallow_solution.h
	g++ -c -g utility/shallow_solution.cpp

clean:
	del *.o cevs
