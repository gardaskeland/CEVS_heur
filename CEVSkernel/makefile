kernel: main.o find_kernel.o read_file.o weighted_graph.o graph.o
	g++ -g -o kernel main.o find_kernel.o read_file.o weighted_graph.o graph.o

main.o: main.cpp find_kernel.h 
	g++ -c -g main.cpp

find_kernel.o: find_kernel.cpp find_kernel.h revert_kernel.h
	g++ -c -g find_kernel.cpp

read_file.o: ../CEVS/read_file.cpp ../CEVS/read_file.h
	g++ -c -g ../CEVS/read_file.cpp

weighted_graph.o: ../CEVS/weighted_graph.cpp ../CEVS/weighted_graph.h
	g++ -c -g ../CEVS/weighted_graph.cpp

graph.o: ../CEVS/graph.cpp ../CEVS/graph.h
	g++ -c -g ../CEVS/graph.cpp

clean:
	rm *.o kernel
