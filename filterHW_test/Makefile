SW: testbench.c
	gcc -g -c -DSW testbench.c
	gcc -o testbench_hw testbench.o -lriffa -lm -lrt -lpthread


clean:
	rm -rf *.o* testbench_hw

PHONY: SW clean
