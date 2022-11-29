#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthreadUtils.h>
#include "tb_utils.h"
#include "uart_interface.h"

void Sender()
{
	double X = 1.0;
	while(1)
	{
		tbSendDouble(X);
		X += 1.0;

		usleep(1000);
	}
}
DEFINE_THREAD(Sender);

int main(int argc, char* argv[])
{
	int I, J;

	if(argc < 3)
	{
		fprintf(stderr,"Usage: %s <tty> <report-interval>\n", argv[0]);
		return(1);
	}

	setUartBlockingFlag(1);
	int tty_fd = setupDebugUartLink(argv[1]);
	if(tty_fd < 0)
	{
		fprintf(stderr,"Error: could not open uart %s\n", argv[1]);
		return (1);
	}


	int interval = atoi(argv[2]);
	fprintf(stderr,"Info: reporting interval = %d\n", interval);

	PTHREAD_DECL(Sender);
	PTHREAD_CREATE(Sender);

	uint8_t B=0;
	int    COUNT=0;
	
	double X = 1.0;
	while(1)
	{
		double Y = tbGetDouble();

		if(Y != (2 * X * X))
		{
			fprintf(stderr,"Error: sent=%lf, received=%lf\n", X, Y);
		}

		COUNT++;
		if((COUNT % interval) == 0)
			fprintf(stderr,"Finished %d.\n", COUNT);
			
		X = X + 1.0;
	}

	return(0);
}
