cevstest: main.o test_solution_representation.o solution_representation.o read_file.o graph.o\
 		test_greedy_merge.o greedy_merge.o read_sol.o test_add_node.o add_node.o weighted_random.o \
		 segment_tree.o test_segment_tree.o test_karger.o karger.o
	g++ -o cevstest main.o test_solution_representation.o solution_representation.o read_file.o\
		graph.o test_greedy_merge.o greedy_merge.o read_sol.o test_add_node.o add_node.o weighted_random.o \
		segment_tree.o test_segment_tree.o test_karger.o karger.o

main.o: main.cpp test_solution_representation.h
	g++ -c -g main.cpp

test_solution_representation.o: test_solution_representation.cpp test_solution_representation.h
	g++ -c -g test_solution_representation.cpp

test_greedy_merge.o: test_ops/test_greedy_merge.cpp test_ops/test_greedy_merge.h
	g++ -c -g test_ops/test_greedy_merge.cpp

test_add_node.o: test_ops/test_add_node.cpp test_ops/test_add_node.h
	g++ -c -g test_ops/test_add_node.cpp

solution_representation.o: ../CEVS/solution_representation.cpp ../CEVS/solution_representation.h
	g++ -c -g ../CEVS/solution_representation.cpp

greedy_merge.o: ../CEVS/operators/greedy_merge.h ../CEVS/operators/greedy_merge.cpp
	g++ -c -g ../CEVS/operators/greedy_merge.cpp

add_node.o: ../CEVS/operators/add_node.h ../CEVS/operators/add_node.cpp
	g++ -c -g ../CEVS/operators/add_node.cpp


read_file.o: ../CEVS/read_file.cpp ../CEVS/read_file.h
	g++ -c -g ../CEVS/read_file.cpp

graph.o: ../CEVS/graph.cpp ../CEVS/graph.h
	g++ -c -g ../CEVS/graph.cpp

read_sol.o: read_sol.cpp read_sol.h
	g++ -c -g read_sol.cpp

weighted_random.o: ../CEVS/utility/weighted_random.cpp ../CEVS/utility/weighted_random.h
	g++ -c -g ../CEVS/utility/weighted_random.cpp

segment_tree.o: ../CEVS/utility/segment_tree.cpp ../CEVS/utility/segment_tree.h
	g++ -c -g ../CEVS/utility/segment_tree.cpp

test_segment_tree.o: test_other/test_segment_tree.cpp test_other/test_segment_tree.h
	g++ -c -g test_other/test_segment_tree.cpp

find_kernel.o: ../CEVSkernel/find_kernel.cpp ../CEVSkernel/find_kernel.h ../CEVSkernel/revert_kernel.h
	g++ -c -g ../CEVSkernel/find_kernel.cpp

karger.o: ../CEVS/utility/karger.cpp ../CEVS/utility/karger.h
	g++ -c -g ../CEVS/utility/karger.cpp

test_karger.o: test_other/test_karger.cpp test_other/test_karger.h
	g++ -c -g test_other/test_karger.cpp

clean:
	del *.o cevstest

