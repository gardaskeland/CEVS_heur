boiler=g++ -std=c++17 -c -g -O

greedy_heuristic: main.o graph.o functions.o binary_search.o
				  g++ -std=c++17 -g -o greedy_heuristic main.o graph.o functions.o binary_search.o

main.o: main.cpp
	$(boiler) main.cpp

graph.o: graph.cpp graph.h
	$(boiler) graph.cpp

functions.o: functions.cpp functions.h 
	$(boiler) functions.cpp

binary_search.o: binary_search.cpp binary_search.h
	$(boiler) binary_search.cpp
	
clean:
	rm *.o greedy_heuristic