gcc -c -g -I../include  hermite.c
gcc -c -g -I./include  testHermite.c
gcc -g -I../include  -o testHermite hermite.o testHermite.o -lm
gcc -g -o innerProduct innerProduct.c -lm
