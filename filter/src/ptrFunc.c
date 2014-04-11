#include <stdio.h>
#include "ptrFunc.h"

/*************************************************
** MEAN CALCULATOR FOR QRS, RR AND NOISE BUFFERS
*************************************************/
int meanCalc(int *buffer)
{
	long sum = 0;
	int i;
	for (i=0; i<8; i=i+1)
	{
		sum += (buffer[i]);	
	}
	return (sum >> 3);
}
