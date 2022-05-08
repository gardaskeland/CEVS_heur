
boiler=g++ -std=c++17 -c -O -g

cevs: test_operators.o solution_representation.o read_file.o graph.o segment_tree.o weighted_graph.o \
	label_propagation.o add_node.o weighted_random.o perturbation.o
	g++ -std=c++17 -g -o test_operators test_operators.o read_file.o graph.o solution_representation.o \
	segment_tree.o weighted_graph.o label_propagation.o add_node.o weighted_random.o perturbation.o

test_operators.o: test_operators.cpp
	$(boiler) test_operators.cpp

read_file.o: ../read_file.cpp ../read_file.h
	$(boiler) ../read_file.cpp

graph.o: ../graph.cpp ../graph.h
	$(boiler) ../graph.cpp

weighted_graph.o: ../weighted_graph.cpp ../weighted_graph.h
	$(boiler) ../weighted_graph.cpp

solution_representation.o: ../solution_representation.cpp ../solution_representation.h
	$(boiler) ../solution_representation.cpp

segment_tree.o: ../utility/segment_tree.cpp ../utility/segment_tree.h
	$(boiler) ../utility/segment_tree.cpp

label_propagation.o: label_propagation.cpp label_propagation.h
	$(boiler) label_propagation.cpp

add_node.o: add_node.cpp add_node.h
	$(boiler) add_node.cpp

perturbation.o: perturbation.cpp perturbation.h
	$(boiler) perturbation.cpp

weighted_random.o: ../utility/weighted_random.cpp ../utility/weighted_random.h
	$(boiler) ../utility/weighted_random.cpp

clean:
	rm *.o cevs
