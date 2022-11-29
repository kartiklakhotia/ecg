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

uint32_t hw_sent_uint8_count = 0;
uint32_t hw_recv_uint8_count = 0;

uint8_t getUint8()
{
	uint8_t ret_val;
	ret_val = read_uint8("in_data");	
#ifdef DEBUGPRINT
	fprintf(stderr,"%d = in_data\n", ret_val);
#endif
	hw_recv_uint8_count++;
	return(ret_val);
}

void	sendUint8(uint8_t X)
{
	write_uint8("out_data", X);	
#ifdef DEBUGPRINT
	fprintf(stderr,"out_data = 0x%x\n", X);
#endif
	hw_sent_uint8_count++;
}


uint16_t getUint16()
{
	uint8_t b0 = getUint8();
	uint8_t b1 = getUint8();

	uint16_t ret_val = b0;
	ret_val = (ret_val << 8) | b1;
	return(ret_val);
}
void	 sendUint16(uint16_t X)
{
	uint8_t b0 = (X >> 8);
	uint8_t b1 = X & 0xff;

	sendUint8(b0);
	sendUint8(b1);
}

uint32_t getUint32()
{
	uint16_t h0 = getUint16();
	uint16_t h1 = getUint16();

	uint32_t ret_val = h0;
	ret_val = (ret_val << 16) | h1;
	return(ret_val);
}

void	 sendUint32(uint32_t X)
{
	uint16_t h0 = (X >> 16);
	uint16_t h1 = (X & 0xffff);

	sendUint16(h0);
	sendUint16(h1);
}

float 	getFloat()
{
	uint32_t uval = getUint32();
	float ret_val = *((float*) &uval);
	return(ret_val);
}

void	sendFloat(float X)
{
	uint32_t sval = *((uint32_t*) &X);
	sendUint32(sval);
}

uint64_t getUint64()
{
	uint32_t u0 = getUint32();
	uint32_t u1 = getUint32();

	uint64_t rval = u0;
	rval  = (rval << 32) | u1;
	return(rval);
}

void	 sendUint64(uint64_t X)
{
	uint32_t u0 = (X >> 32);
	uint32_t u1 = (X & 0xffffffff);

	sendUint32(u0);
	sendUint32(u1);
}

double 	 getDouble()
{
	uint64_t u = getUint64();
	double rval = *((double*) &u);
	return(rval);
}

void	 sendDouble(double X)
{
	uint64_t u = *((uint64_t*) &X);
	sendUint64(u);
}

