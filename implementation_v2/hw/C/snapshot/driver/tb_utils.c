#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthreadUtils.h>
#include <Pipes.h>
#include <pipeHandler.h>
#include "app_defines.h"
#include "hermite.h"
#include "best_fit.h"
#include "utils.h"
#ifndef SW
#ifdef  FPGA
#include "uart_interface.h"
#else
#include "vhdlCStubs.h"
#endif
#endif

uint32_t tb_sent_uint8_count = 0;
uint32_t tb_recv_uint8_count = 0;

uint8_t tbGetUint8()
{
	uint8_t ret_val;
#ifdef FPGA
	#ifdef FREADWRITE
		fread((void*) &ret_val, 1, 1, stdin);
	#else
		ret_val = recvByte();
	#endif
#else
	ret_val = read_uint8("out_data");
#endif

#ifdef DEBUGPRINT
	fprintf(stderr,"tb: %d = out_data\n", ret_val);
#endif
	tb_recv_uint8_count++;
	return(ret_val);
}

void	tbSendUint8(uint8_t X)
{
#ifdef FPGA
	#ifdef FREADWRITE
		fwrite(&X, 1,1 , stdout);
	#else
		while(1)
		{
			int n = sendByte(X);
			if(n==1)
				break;
		}
	#endif
#else
	write_uint8("in_data", X);	
#endif

#ifdef DEBUGPRINT
	fprintf(stderr,"tb: in_data = %d \n", X);
#endif
	tb_sent_uint8_count++;
}


uint16_t tbGetUint16()
{
	uint8_t b0 = tbGetUint8();
	uint8_t b1 = tbGetUint8();

	uint16_t ret_val = b0;
	ret_val = (ret_val << 8) | b1;
	return(ret_val);
}
void	 tbSendUint16(uint16_t X)
{
	uint8_t b0 = (X >> 8);
	uint8_t b1 = X & 0xff;

	tbSendUint8(b0);
	tbSendUint8(b1);
}

uint32_t tbGetUint32()
{
	uint16_t h0 = tbGetUint16();
	uint16_t h1 = tbGetUint16();

	uint32_t ret_val = h0;
	ret_val = (ret_val << 16) | h1;
	return(ret_val);
}

void	 tbSendUint32(uint32_t X)
{
	uint16_t h0 = (X >> 16);
	uint16_t h1 = (X & 0xffff);

	tbSendUint16(h0);
	tbSendUint16(h1);
}

float 	tbGetFloat()
{
	uint32_t uval = tbGetUint32();
	float ret_val = *((float*) &uval);
	return(ret_val);
}

void	tbSendFloat(float X)
{
	uint32_t sval = *((uint32_t*) &X);
	tbSendUint32(sval);
}

uint64_t tbGetUint64()
{
	uint32_t u0 = tbGetUint32();
	uint32_t u1 = tbGetUint32();

	uint64_t rval = u0;
	rval  = (rval << 32) | u1;
	return(rval);
}

void	 tbSendUint64(uint64_t X)
{
	uint32_t u0 = (X >> 32);
	uint32_t u1 = (X & 0xffffffff);

	tbSendUint32(u0);
	tbSendUint32(u1);
}

double 	 tbGetDouble()
{
	uint64_t u = tbGetUint64();
	double rval = *((double*) &u);
	return(rval);
}

void	 tbSendDouble(double X)
{
	uint64_t u = *((uint64_t*) &X);
	tbSendUint64(u);
}




