boiler=g++ -std=c++17 -c -g -O

greedy_heuristic: main.o graph_.o functions.o binary_search.o graph.o solution_representation.o segment_tree.o
				  g++ -std=c++17 -g -o greedy_heuristic main.o graph_.o functions.o binary_search.o graph.o \
				  solution_representation.o segment_tree.o

main.o: main.cpp
	$(boiler) main.cpp

graph_.o: graph_.cpp graph_.h
	$(boiler) graph_.cpp

functions.o: functions.cpp functions.h 
	$(boiler) functions.cpp

binary_search.o: binary_search.cpp binary_search.h
	$(boiler) binary_search.cpp

graph.o: ../CEVS/graph.cpp ../CEVS/graph.h
	$(boiler) ../CEVS/graph.cpp

solution_representation.o: ../CEVS/solution_representation.cpp ../CEVS/solution_representation.h
	$(boiler) ../CEVS/solution_representation.cpp

segment_tree.o: ../CEVS/utility/segment_tree.cpp ../CEVS/utility/segment_tree.h
	$(boiler) ../CEVS/utility/segment_tree.cpp
	
clean:
	rm *.o greedy_heuristic