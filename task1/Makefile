FLAGS = -Wall -Wextra -Wpedantic -O3
#FLAGS = -Wall -Wextra -Wpedantic -O0

.PHONY: all clean

all: main

clean:
	rm *.o main

grid.o: grid.c
	gcc -c -o grid.o grid.c $(FLAGS)

util.o: util.c
	gcc -c -o util.o util.c $(FLAGS)

sequential_alg.o: sequential_alg.c
	gcc -c -o sequential_alg.o sequential_alg.c $(FLAGS)

parallel_alg.o: parallel_alg.c
	gcc -c -o parallel_alg.o parallel_alg.c $(FLAGS) -fopenmp 
main.o: main.c
	gcc -c -o main.o main.c $(FLAGS) -lm

main: main.o grid.o util.o sequential_alg.o parallel_alg.o
	gcc -o main main.o grid.o util.o sequential_alg.o parallel_alg.o $(FLAGS) -fopenmp -lm
