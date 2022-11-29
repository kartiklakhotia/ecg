gcc -g -o test_filter test_filter.c
gcc -g -o expand_hermite_fit expand_hermite_fit.c hermite.c  -I ../include -I ./ -D SW -D USE_DOUBLE -lm
gcc -g -o gen_hermite  gen_hermite.c hermite.c  -I ../include -I ./ -D SW -D USE_DOUBLE -lm
gcc -g -o brute_force_hermite  brute_force_hermite.c hermite.c  -I ../include -I ./ -D SW -D USE_DOUBLE -lm
gcc -g -o brute_force_hermite_one_beat -D ONEBEAT  brute_force_hermite.c hermite.c  -I ../include -I ./ -D SW -D USE_DOUBLE -lm
