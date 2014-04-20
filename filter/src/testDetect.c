#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthreadUtils.h>
#include <Pipes.h>
#include <pipeHandler.h>
#include "filter.h"
#include "qrsDet.h"
#ifndef SW
#include "vhdlCStubs.h"
#endif


#ifdef SW
DEFINE_THREAD(qrsDet)
#endif

int main(int argc, char* argv[])
{
	long QRSdelay;
	if (argc < 2)
	{
		fprintf(stderr, "Supply data record file\n");
		return(1);
	}
	
	FILE* frec = fopen(argv[1], "r");
	FILE* fout = fopen("delay.txt", "w");
	if(frec == NULL)
	{
		fprintf(stderr, "could not open the record file %s\n", argv[1]);
		return(1);
	}
#ifdef SW
	init_pipe_handler();
	PTHREAD_DECL(qrsDet);
	PTHREAD_CREATE(qrsDet);
#endif
//	write_uint64("det_input_pipe", 0);
//	QRSdelay = read_uint64("det_output_pipe");
	long dataSample;
	fscanf(frec, "%ld", &dataSample);
	long count = 0;
	while(!feof(frec)){
		count++;
		write_uint64("det_input_pipe", dataSample);
		QRSdelay = read_uint64("det_output_pipe");
#ifdef HW
		printf("value received for QRS delay is %ld\n", QRSdelay);
#endif
		if (1)//QRSdelay != 0)
		{
			fprintf(fout, "%ld, ", QRSdelay);
			fprintf(fout, "%ld\n", count);
//			fprintf(fout, "%ld\n", QRSdelay - count);
		}
//			fprintf(fout, "%ld\n", count);
		fscanf(frec, "%ld", &dataSample);
	}
	fclose(frec);
	fclose(fout);
	
#ifdef SW
	PTHREAD_CANCEL(qrsDet);
	close_pipe_handler();
	return(0);
#endif
}
