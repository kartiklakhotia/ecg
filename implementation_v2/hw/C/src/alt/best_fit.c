#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <Pipes.h>
#include <stdio.h>
#include "app_defines.h"
#include "data_structs.h"
#include "best_fit.h"
#include "utils.h"
#include "storage.h"
#include "fpops.h"

extern HermiteFit best_hermite_fit;
extern REAL current_beat[144];

REAL best_mse;
int32_t best_sigma_index;

#ifdef SW
void __loop_pipelining_on__(int A,int B,int C) {}
#else
void __loop_pipelining_on__(int,int,int); 
#endif	

#ifdef SW
	REAL beat_energy = 0;
#endif

REAL hf_storage[NHERMITE*MEM_SIZE_PER_HF];
REAL dotp_storage[NHERMITE*NSIGMAS];




double     err[NSIGMAS]; 


#ifdef USE_DOUBLE
double getHfValue()
{
	double rval = getDouble();
#ifdef READBACK
	sendDouble(rval);
#endif
	return(rval);
}
#else
float getHfValue()
{
	float rval = getFloat();
#ifdef READBACK
	sendFloat(rval);
#endif
	return(rval);
}
#endif


//
// Hermite polynomials are fed in by sending in the
// polynomial values for the distinct values of sigma.
//
void initializeHermitePolynomials()
{
	int idx;

	for (idx = 0; idx < MEM_SIZE_PER_HF*NHERMITE; idx++){
		hf_storage[idx] = getHfValue();
	}
#ifdef SW
	fprintf(stderr,"Info: in hardware, read Hermite polynomials from the environment.\n");
#endif
}

void initBestFit()
{
	int idx;

	for(idx = 0; idx < NHERMITE*NSIGMAS; idx++)
	{
		//__loop_pipelining_on__(7,2,0);
		dotp_storage[idx] = 0;	
		if(idx < NSIGMAS)
		{
			err[idx] = 0;
		}
	}

#ifdef SW
	fprintf(stderr, "Info: fit initialized.\n");
#endif
}


// given the next input, the hf polynomial
// array and the dotp array, update the dotp
// arrays for all sigma positions with 
// the inner product computation.
// 
// 
void  __InnerProduct__(int I) {
	int SI, H;
	REAL X = current_beat[I];
	for(SI = 0; SI < NSIGMAS; SI++)
	{
		for(H = 0; H < NHERMITE; H++)
		{
#ifdef USE_DOUBLE
			REAL p = fp64_mul_for_ecg(X, __hf_mem__(H,SI,I));
			REAL m = __dotp_mem__(H,SI);
			__dotp_mem__(H,SI) = fp64_addsub_for_ecg(m,p,0);
#else
			REAL p = fp32_mul_for_ecg(X, __hf_mem__(H,SI,I));
			REAL m = __dotp_mem__(H,SI);
			__dotp_mem__(H,SI) = fp32_addsub_for_ecg(m,p,0);
#endif

		}
	}
}


//
// compute the inner products with all the hF basis
// polynomials.
//
void ComputeInnerProducts()
{
	int I;
	REAL x;
	for (I=0; I<NSAMPLES; I++){
		__InnerProduct__(I);
	}
#ifdef SW
	for(I = 0; I < NSIGMAS; I++)
	{
		int H;
		for(H = 0; H < NHERMITE; H++)
		{
			fprintf(stderr,"HW: Dot-product for sigma-index %d, HF %d, is %le.\n", 
						I, H, __dotp_mem__(H,I));
		}
	}
#endif

}


// For each sigma:
//  Calculate the projection of the input
//  data  onto the subspace spanned by the
//  hF's.  
//  
//  Find the mse of the difference between
//  the projection and the original data.
//
//  Keep track of the smallest mse and the
//  corresponding sigma index.
//
void computeMSE()
{
	int I, SI;

	best_mse = 1.0e+50;
	best_sigma_index = -1;

	for (I=0; I<NSAMPLES; I++)
	{
		for (SI=0; SI<NSIGMAS; SI++)
		{
			REAL P = 0;
			int H;
			for(H = 0; H < NHERMITE; H++)
			{
			// Not much point right now.
			//__loop_pipelining_on__(15,2,0);
			//
#ifdef USE_DOUBLE
				P  = fp64_addsub_for_ecg(P,  fp64_mul_for_ecg ( __dotp_mem__(H,SI) , __hf_mem__(H,SI,I)), 0);
#else
				P  = fp32_addsub_for_ecg(P,  fp32_mul_for_ecg ( __dotp_mem__(H,SI) , __hf_mem__(H,SI,I)), 0);
#endif

			}
#ifdef USE_DOUBLE
			REAL diff = fp64_addsub_for_ecg (current_beat[I], P, 1);
			err[SI] +=  fp64_mul_for_ecg(diff , diff);
#else
			REAL diff = fp32_addsub_for_ecg (current_beat[I], P, 1);
			err[SI] +=  fp32_mul_for_ecg(diff , diff);
#endif
		}
	}
	for (SI=0; SI<NSIGMAS; SI++)
	{
#ifdef SW
		fprintf(stderr,"HW: Error for %d-th sigma is %le (relative=%le).\n", 
						SI, err[SI], 
						err[SI]/beat_energy);
#endif
		if(err[SI] <  best_mse)
		{
			best_mse = err[SI];
			best_sigma_index = SI;
		}
	}

}


void findBestFit()
{
	initBestFit();

#ifdef MONITORBACKEND
	write_uint32("backend_monitor_pipe", 0xf0000001);
#endif

#ifdef SW
	int J;
	beat_energy = 0;
	for(J = 0; J < NSAMPLES; J++)
	{
		beat_energy += (current_beat[J] * current_beat[J]);
	}
#endif

	
	// Beat is available in current_beat.  Compute the
	// inner produts for all sigma's
	ComputeInnerProducts();

#ifdef MONITORBACKEND
	write_uint32("backend_monitor_pipe", 0xf0000002);
#endif

	// At this point you have the projections.  Calculate
	// MSE for each projection..
	computeMSE();

	best_hermite_fit.best_mse = best_mse;
	best_hermite_fit.best_sigma_index = best_sigma_index;

#ifdef MONITORBACKEND
	write_uint32("backend_monitor_pipe", 0xf0000003);
#endif

	// At this point, we have the best SI.
	int I = best_hermite_fit.best_sigma_index;
	int H = 0;
	for(H = 0; H < NHERMITE; H++)
	{
		best_hermite_fit.hermite_coefficients[H] = __dotp_mem__(H,I);
	}
#ifdef MONITORBACKEND
	write_uint32("backend_monitor_pipe", 0xf0000004);
#endif
}
