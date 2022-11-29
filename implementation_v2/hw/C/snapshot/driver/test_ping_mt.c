#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthreadUtils.h>
#include "uart_interface.h"

int interval = 1024;

void receiver()
{
	uint8_t B;
	int     COUNT=0;
	while(1)
	{
		uint8_t C = recvByte();

		if(C != B)
		{
			fprintf(stderr,"Error: received=0x%x, expected 0x%x\n", C, B);
		}

		B++;
		COUNT++;

		if((COUNT % interval) == 0)
			fprintf(stderr,"Finished %d  bytes\n", COUNT);
	}
}
DEFINE_THREAD(receiver);


int main(int argc, char* argv[])
{
	int I, J;

	if(argc < 3)
	{
		fprintf(stderr,"Usage: %s <tty> <report-interval>\n", argv[0]);
		return(1);
	}

	setUartBlockingFlag(0);
	int tty_fd = setupDebugUartLink(argv[1]);
	if(tty_fd < 0)
	{
		fprintf(stderr,"Error: could not open uart %s\n", argv[1]);
		return (1);
	}


	interval = atoi(argv[2]);
	fprintf(stderr,"Info: reporting interval = %d\n", interval);

	PTHREAD_DECL(receiver);
	PTHREAD_CREATE(receiver);

	uint8_t B=0;
	while(1)
	{
		sendByte(B);
		B++;
		usleep(1000000);
	}

	return(0);
}
