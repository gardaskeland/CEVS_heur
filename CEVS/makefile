
boiler=g++ -std=c++17 -c -Og -g

cevs: main.o read_file.o graph.o solution_representation.o add_node.o \
		greedy_merge.o local_search.o weighted_random.o bookkeep.o \
		b_merge.o shallow_solution.o segment_tree.o b_add_node.o weighted_graph.o \
		find_kernel.o split.o karger.o simulated_annealing.o local_search_lp.o \
		label_propagation.o perturbation.o alns.o logging_solution.o inout.o
	g++ -std=c++17 -g -o cevs main.o read_file.o graph.o solution_representation.o add_node.o \
		greedy_merge.o local_search.o weighted_random.o bookkeep.o b_merge.o \
		shallow_solution.o segment_tree.o b_add_node.o weighted_graph.o \
		find_kernel.o split.o karger.o simulated_annealing.o local_search_lp.o \
		label_propagation.o perturbation.o alns.o logging_solution.o inout.o

main.o: main.cpp
	$(boiler) main.cpp

read_file.o: read_file.cpp read_file.h
	$(boiler) read_file.cpp

inout.o: inout.cpp inout.h
	$(boiler) inout.cpp

graph.o: graph.cpp graph.h
	$(boiler) graph.cpp

solution_representation.o: solution_representation.cpp solution_representation.h
	$(boiler) solution_representation.cpp

add_node.o: operators/add_node.cpp operators/add_node.h
	$(boiler) operators/add_node.cpp

greedy_merge.o: operators/greedy_merge.cpp operators/greedy_merge.h
	$(boiler) operators/greedy_merge.cpp

local_search.o: metaheuristics/local_search.cpp metaheuristics/local_search.h
	$(boiler) metaheuristics/local_search.cpp

weighted_random.o: utility/weighted_random.cpp utility/weighted_random.h
	$(boiler) utility/weighted_random.cpp

bookkeep.o: bookkeep/bookkeep.cpp bookkeep/bookkeep.h
	$(boiler) bookkeep/bookkeep.cpp

b_merge.o: bookkeep/b_merge.cpp bookkeep/b_merge.h
	$(boiler) bookkeep/b_merge.cpp

b_add_node.o: bookkeep/b_add_node.cpp bookkeep/b_add_node.h
	$(boiler) bookkeep/b_add_node.cpp

shallow_solution.o: utility/shallow_solution.cpp utility/shallow_solution.h
	$(boiler) utility/shallow_solution.cpp

segment_tree.o: utility/segment_tree.cpp utility/segment_tree.h
	$(boiler) utility/segment_tree.cpp

weighted_graph.o: weighted_graph.cpp weighted_graph.h
	$(boiler) weighted_graph.cpp

find_kernel.o: ../CEVSkernel/find_kernel.cpp ../CEVSkernel/find_kernel.h ../CEVSkernel/revert_kernel.h
	$(boiler) ../CEVSkernel/find_kernel.cpp

split.o: operators/split.cpp operators/split.h
	$(boiler) operators/split.cpp

karger.o: utility/karger.cpp utility/karger.h
	$(boiler) utility/karger.cpp

simulated_annealing.o: metaheuristics/simulated_annealing.cpp metaheuristics/simulated_annealing.h
	$(boiler) metaheuristics/simulated_annealing.cpp

label_propagation.o: operators/label_propagation.cpp operators/label_propagation.h
	$(boiler) operators/label_propagation.cpp

local_search_lp.o: metaheuristics/local_search_lp.cpp metaheuristics/local_search_lp.h
	$(boiler) metaheuristics/local_search_lp.cpp

perturbation.o: operators/perturbation.cpp operators/perturbation.h
	$(boiler) operators/perturbation.cpp

alns.o: metaheuristics/alns.cpp metaheuristics/alns.h
	$(boiler) metaheuristics/alns.cpp

logging_solution.o: utility/logging_solution.cpp utility/logging_solution.h
	$(boiler) utility/logging_solution.cpp

clean:
	rm *.o cevs
