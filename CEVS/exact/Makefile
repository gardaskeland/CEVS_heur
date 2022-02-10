boiler=g++ -std=c++17 -c -g -O

exact: exact.o read_file.o graph.o solution_representation.o segment_tree.o
	g++ -std=c++17 -g -o exact exact.o read_file.o graph.o solution_representation.o segment_tree.o

main.o: exact.cpp
	$(boiler) exact.cpp

read_file.o: ../read_file.cpp ../read_file.h
	$(boiler) ../read_file.cpp

solution_representation.o: ../solution_representation.cpp ../solution_representation.h
	$(boiler) ../solution_representation.cpp

graph.o: ../graph.cpp ../graph.h
	$(boiler) ../graph.cpp

segment_tree.o: ../utility/segment_tree.cpp ../utility/segment_tree.h
	$(boiler) ../utility/segment_tree.cpp

clean:
	rm *.o exact

