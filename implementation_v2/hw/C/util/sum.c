#include <stdio.h>
#include <stdint.h>
#include <math.h>


int main (int argc, char* argv[])
{

	double sum = 0;
	while (!feof(stdin))
	{
	
		int32_t val;
		int n = fscanf(stdin,"%d", &val);	

		fprintf(stderr,"in: %d\n", val);

		if((n == 0) || (n == EOF))
			break;

		sum += (double) val;
		fprintf(stdout,"%lf\n", sum);
	}

	return(0);
}

