boiler=g++ -std=c++17 -c -O -g

cevstest: main.o test_solution_representation.o solution_representation.o read_file.o graph.o\
 		test_greedy_merge.o greedy_merge.o read_sol.o test_add_node.o add_node.o weighted_random.o \
		 segment_tree.o test_segment_tree.o gard_random.o test_kernel.o
	g++ -o cevstest main.o test_solution_representation.o solution_representation.o read_file.o\
		graph.o test_greedy_merge.o greedy_merge.o read_sol.o test_add_node.o add_node.o weighted_random.o \
		segment_tree.o test_segment_tree.o gard_random.o test_kernel.o

main.o: main.cpp test_solution_representation.h
	$(boiler) main.cpp

test_solution_representation.o: test_solution_representation.cpp test_solution_representation.h
	$(boiler) test_solution_representation.cpp

test_greedy_merge.o: test_ops/test_greedy_merge.cpp test_ops/test_greedy_merge.h
	$(boiler) test_ops/test_greedy_merge.cpp

test_add_node.o: test_ops/test_add_node.cpp test_ops/test_add_node.h
	$(boiler) test_ops/test_add_node.cpp

solution_representation.o: ../CEVS/solution_representation.cpp ../CEVS/solution_representation.h
	$(boiler) ../CEVS/solution_representation.cpp

greedy_merge.o: ../CEVS/operators/greedy_merge.h ../CEVS/operators/greedy_merge.cpp
	$(boiler) ../CEVS/operators/greedy_merge.cpp

add_node.o: ../CEVS/operators/add_node.h ../CEVS/operators/add_node.cpp
	$(boiler) ../CEVS/operators/add_node.cpp

read_file.o: ../CEVS/read_file.cpp ../CEVS/read_file.h
	$(boiler) ../CEVS/read_file.cpp

graph.o: ../CEVS/graph.cpp ../CEVS/graph.h
	$(boiler) ../CEVS/graph.cpp

read_sol.o: read_sol.cpp read_sol.h
	$(boiler) read_sol.cpp

weighted_random.o: ../CEVS/utility/weighted_random.cpp ../CEVS/utility/weighted_random.h
	$(boiler) ../CEVS/utility/weighted_random.cpp

gard_random.o: ../CEVS/utility/gard_random.cpp ../CEVS/utility/gard_random.h
	$(boiler) ../CEVS/utility/gard_random.cpp

segment_tree.o: ../CEVS/utility/segment_tree.cpp ../CEVS/utility/segment_tree.h
	$(boiler) ../CEVS/utility/segment_tree.cpp

test_segment_tree.o: test_other/test_segment_tree.cpp test_other/test_segment_tree.h
	$(boiler) test_other/test_segment_tree.cpp

find_kernel.o: ../CEVSkernel/find_kernel.cpp ../CEVSkernel/find_kernel.h ../CEVSkernel/revert_kernel.h
	$(boiler) ../CEVSkernel/find_kernel.cpp

test_kernel.o: test_kernel/test_kernel.cpp test_kernel/test_kernel.h
	$(boiler) test_kernel/test_kernel.cpp

clean:
	rm *.o cevstest

