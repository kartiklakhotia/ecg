#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "fpops.h"
#ifdef AA2C
#include "ajit_fpu_aa_c_model.h"
#endif

void dcompare(double A, double B)
{
	if(A != B)
	{
		fprintf(stderr,"dcompare!error! %le %le\n", A, B);
	}	
}
void fcompare(float A, float B)
{
	if(A != B)
	{
		fprintf(stderr,"fcompare!error! %e %e\n", A, B);
	}
}

double fp64_mul_for_ecg(double X, double Y)
{
#ifdef AA2C
	double retval;
	ajit_fpu_fp64_mul_for_ecg(X,Y,&retval);
	
	dcompare(retval, X*Y);
	return(retval);
#else
	return(X*Y);
#endif
}
double fp64_addsub_for_ecg(double X, double Y, uint8_t sub_flag)
{
#ifdef AA2C
	double retval;
	ajit_fpu_fp64_addsub_for_ecg(X,Y, sub_flag, &retval);
	dcompare(retval, (X + ((sub_flag ==0) ? Y : -Y)));

	return(retval);
#else
	return (X + ((sub_flag ==0) ? Y : -Y));
#endif
}

float fp32_mul_for_ecg(float X, float Y)
{
#ifdef AA2C
	float retval;
	ajit_fpu_fp32_mul_for_ecg(X,Y,&retval);
	fcompare(retval, X*Y);
	return(retval);
#else
	return(X*Y);
#endif
}

float fp32_addsub_for_ecg(float X, float Y, uint8_t sub_flag)
{
#ifdef AA2C
	float retval;
	ajit_fpu_fp32_addsub_for_ecg(X,Y, sub_flag, &retval);
	fcompare(retval, (X + ((sub_flag ==0) ? Y : -Y)));

	return(retval);
#else
	return (X + ((sub_flag ==0)  ? Y : -Y));
#endif
}

