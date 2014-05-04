#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <riffa.h>


fpga_t * fpga;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Supply data record file \n");
		return(1);
	}
	long *QRSdelay;
	QRSdelay = (long *) malloc(sizeof(long));
	FILE* frec = fopen(argv[1], "r");
	FILE* fout = fopen("delay.txt", "w");
	if (frec == NULL)
	{
		fprintf(stderr, "could not open the record file %s\n", argv[1]);
		return(1);
	}
	fpga = fpga_open(0);
	fpga_reset(fpga);
	long *datum;
	datum = (long *) malloc(sizeof(long));
	int sampleSent, recv;
	sampleSent = fpga_send(fpga, 0, datum, 2, 0, 1, 0);
	recv = fpga_recv(fpga, 0, QRSdelay, 2, 0);
	fscanf(frec, "%ld", datum);
	long count = 1;
	while(!feof(frec))
	{
		count++;
		sampleSent = fpga_send(fpga,0,datum,1, 0, 1, 1000);
		if (sampleSent < 1)
		{
			printf("in loop # %ld, the data sample was not sent", count);
			break;
		}
		recv  = fpga_recv(fpga, 0, QRSdelay, 1, 1000);
		if (recv < 1)
		{
			printf("in loop # %ld, the delay output was not received", count);
			break;
		}
		if (*QRSdelay != 0)
		{
			fprintf(fout, "%ld, ", *QRSdelay);
			fprintf(fout, "%ld\n", count);
		}
		fscanf(frec, "%ld", datum);
	}	
	fclose(frec);
	fclose(fout);
	fpga_close(fpga);
	return (0);
}		
			
