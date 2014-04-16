#include <stdio.h>
#include "ptrFunc.h"

/*************************************************
** MEAN CALCULATOR FOR QRS, RR AND NOISE BUFFERS
*************************************************/
int meanCalc(int *buff)
{
	long sum = 0;
	int i;
	for (i=0; i<8; i=i+1)
	{
		sum += (buff[i]);	
	}
	return (sum >> 3);
}
