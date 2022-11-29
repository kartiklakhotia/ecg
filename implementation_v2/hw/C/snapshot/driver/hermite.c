#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hermite.h"
#include "best_fit.h"
#include "config.h"
#include "tb_utils.h"


extern Config hw_config;

void  __HF__(int N, int SI, REAL* hhf) 
{
	int idx;

	REAL hF[NSAMPLES];
	REAL sigma = MIN_SIGMA + SI*((MAX_SIGMA-MIN_SIGMA)/NSIGMAS);

	normalizedHermiteBasisFunction(N,sigma,hF);

	for(idx	= 0; idx < NSAMPLES; idx++)
	{
		REAL oF = hF[idx];
		hhf[N*MEM_SIZE_PER_HF + (SI*NSAMPLES) + idx] = oF;
	}
}
	

void setAllOnesInHermitePolynomials()
{
	int I;
	for(I=0; I < MEM_SIZE_PER_HF * NHERMITE; I++)
	{
		hw_config.hF[I] = 1.0;
	}
}

void fillHermitePolynomialsInConfig()
{
	
	fprintf(stderr,"Info: filling herrmite polynomials\n");
	REAL sigma;
	int SI, H, idx;
	for(H =0; H < NHERMITE; H++)
	{
		for(SI = 0; SI < NSIGMAS; SI++)
		{
			__HF__(H,SI,(hw_config.hF));
		}
	}
	fprintf(stderr," filled hermite polynomials.\n");
}


// N = order
// sigma = scale factor.
// x = point at which the polynomial is to be computed.
REAL hermitePolynomial(int N, REAL x)
{
	REAL ret_val = 0.0;
	if(N == 0)
		ret_val = 1;
	else if(N == 1)
		ret_val = 2.0 * x;
	else 
	{
		REAL H_1 = (2.0 * x * hermitePolynomial(N-1, x));
		REAL H_2 = (2.0 * (N-1) * hermitePolynomial(N-2, x));
		ret_val = H_1 - H_2;
	}
	return(ret_val);
}

REAL nFactor(int N)
{
	REAL ret_val = 1.0;
	if(N > 0)
	{
		ret_val = nFactor(N-1) * 1.0 / sqrt(2 * N);
	}
	return(ret_val);
}

REAL basisScaleFactor(int N, REAL sigma, REAL x)
{
	REAL exp_factor  = pow(M_E, -(pow(x/sigma,2.0)/ 2.0));
	REAL denom_1 = 1.0/sqrt(sigma * sqrt(M_PI));
	REAL denom_2 = nFactor(N);
	return(exp_factor * denom_1 * denom_2);
}

REAL hermiteBasisFunction(int N, REAL sigma, REAL x)
{
	REAL ret_val = basisScaleFactor(N,sigma,x) *  hermitePolynomial(N,x/sigma);
	return(ret_val);
}

void normalizedHermiteBasisFunction(int N, REAL sigma, REAL* nhf)
{
	REAL norm_sq = 0.0;
	int idx;
	for(idx	= 0; idx < NSAMPLES; idx++)
	{
		REAL X = ((idx - (NSAMPLES/2)) * SAMPLING_INTERVAL);
		REAL oF = hermiteBasisFunction(N,sigma,X);
		nhf[idx] = oF;
		norm_sq += oF * oF;
	}
	
	REAL norm = sqrt(norm_sq);
	for(idx	= 0; idx < NSAMPLES; idx++)
	{
		nhf[idx] /= norm;
	}
}

