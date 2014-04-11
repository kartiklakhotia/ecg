SW: hermite.c bestFit.c hermite.h bestFit.h  
	gcc -g -c -DSW hermite.c
	gcc -g -c -DSW bestFit.c
	gcc -g -c -DSW ecgSW.c
	gcc -o testbench_sw ecgSW.o hermite.o bestFit.o -lriffa -lm -lrt -lpthread


clean:
	rm -rf *.o* testbench_sw

PHONY: all clean	
