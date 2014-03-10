#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <Pipes.h>
#include <stdio.h>
#include <timer.h>
#include "bestFit.h"

#ifdef SW
void __loop_pipelining_on__(int A,int B,int C) {}
#else
void __loop_pipelining_on__(int,int,int); 
#endif	

double best_mse;
int best_sigma_index ;

void initHF()
{
	int M = MEM_SIZE;
	int idx;
	int shift1, shift2, shift3, shift4, shift5;
	shift1 = M;
	shift2 = 2*M;
	shift3 = 3*M;
	shift4 = 4*M;
	shift5 = 5*M;
	for(idx = 0; idx < NSIGMAS; idx++)
	{
		__loop_pipelining_on__(8,2,0);
		dotP0[idx]  = 0;
		dotP1[idx]  = 0;
		dotP2[idx]  = 0;
		dotP3[idx]  = 0;
		dotP4[idx]  = 0;
		dotP5[idx]  = 0;
		err[idx] = 0;
		Offset[idx] = NSAMPLES*idx;
	}

	for (idx = 0; idx < 6*M; idx++){
		hFall[idx] = read_float64("in1_data");
	}

	for(idx = 0; idx < M; idx++)
	{
		__loop_pipelining_on__(15,2,0);
		hF0[idx] = hFall[idx];
		hF1[idx] = hFall[shift1+idx];
		hF2[idx] = hFall[shift2+idx];	
		hF3[idx] = hFall[shift3+idx];
		hF4[idx] = hFall[shift4+idx];
		hF5[idx] = hFall[shift5+idx];
	}
#ifdef SW
	fprintf(stderr, "reading Hermite function 6 vals completed\n");
#endif

#ifdef SW
	fprintf(stderr,"HW: initHF completed.\n");
#endif
}

void initFit()
{
	int idx;

	for(idx = 0; idx < NSIGMAS; idx++)
	{
		__loop_pipelining_on__(8,2,0);
		dotP0[idx]  = 0;
		dotP1[idx]  = 0;
		dotP2[idx]  = 0;
		dotP3[idx]  = 0;
		dotP4[idx]  = 0;
		dotP5[idx]  = 0;
		err[idx] = 0;
	}
}


// given the next input, the hf polynomial
// array and the dotp array, update the dotp
// arrays for all sigma positions with 
// the inner product computation.
// 
// 
inline void  __InnerProduct__(int I) {
	int SI;
	int secondIndex = I + 1;
	int thirdIndex = I + 2;
	int fourthIndex = I + 3;
	double x0 = inputData[I];
	double x1 = inputData[secondIndex];
	double x2 = inputData[thirdIndex];
	double x3 = inputData[fourthIndex];
	for(SI = 0; SI < NSIGMAS; SI++)
	{
		__loop_pipelining_on__(15,2,0);
		int I0 = I + Offset[SI];
		int I1 = secondIndex + Offset[SI];
		int I2 = thirdIndex + Offset[SI];
		int I3 = fourthIndex + Offset[SI];

		double p00 = (x0*hF0[I0]);
		double p10 = (x0*hF1[I0]);
		double p20 = (x0*hF2[I0]);
		double p30 = (x0*hF3[I0]);
		double p40 = (x0*hF4[I0]);
		double p50 = (x0*hF5[I0]);

		double p01 = (x1*hF0[I1]);
		double p11 = (x1*hF1[I1]);
		double p21 = (x1*hF2[I1]);
		double p31 = (x1*hF3[I1]);
		double p41 = (x1*hF4[I1]);
		double p51 = (x1*hF5[I1]);

		double p02 = (x2*hF0[I2]);
		double p12 = (x2*hF1[I2]);
		double p22 = (x2*hF2[I2]);
		double p32 = (x2*hF3[I2]);
		double p42 = (x2*hF4[I2]);
		double p52 = (x2*hF5[I2]);

		double p03 = (x3*hF0[I3]);
		double p13 = (x3*hF1[I3]);
		double p23 = (x3*hF2[I3]);
		double p33 = (x3*hF3[I3]);
		double p43 = (x3*hF4[I3]);
		double p53 = (x3*hF5[I3]);

		dotP0[SI] += (p00+p01) + (p02 + p03);
		dotP1[SI] += (p10+p11) + (p12 + p13);
		dotP2[SI] += (p20+p21) + (p22 + p23);
		dotP3[SI] += (p30+p31) + (p32 + p33);
		dotP4[SI] += (p40+p41) + (p42 + p43);
		dotP5[SI] += (p50+p51) + (p52 + p53);
	}

}


// receive a sample frame and compute the
// inner products with all the hF basis
// polynomials.
void RxAndComputeInnerProducts()
{
	int I;
	double x;
	for(I=0; I < NSAMPLES; I++)
	{
		__loop_pipelining_on__(8,2,0);
		x = read_float64("in0_data");
		inputData[I] = x;

		// note: the sample interval will about 
		// 1ms.  Assuming a clock of 100MHz, this
		// means that the inner-product loop below
		// should finish in 100K cycles.  not likely
		// to be critical.
	}
	for (I=0; I<NSAMPLES; I=I+4){
//		__loop_pipelining_on__(15,2,0);
		__InnerProduct__(I);
	}
#ifdef SW
#ifdef debug
	for(I = 0; I < NSIGMAS; I++)
	{
		fprintf(stderr,"HW: Dot-product for sigma-index %d, HF-0, is %f.\n", I, dotP0[I]);
		fprintf(stderr,"HW: Dot-product for sigma-index %d, HF-1, is %f.\n", I, dotP1[I]);
		fprintf(stderr,"HW: Dot-product for sigma-index %d, HF-2, is %f.\n", I, dotP2[I]);
		fprintf(stderr,"HW: Dot-product for sigma-index %d, HF-3, is %f.\n", I, dotP3[I]);
		fprintf(stderr,"HW: Dot-product for sigma-index %d, HF-4, is %f.\n", I, dotP4[I]);
		fprintf(stderr,"HW: Dot-product for sigma-index %d, HF-5, is %f.\n", I, dotP5[I]);
	}
#endif
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

	best_mse = 1.0e+20;
	best_sigma_index = -1;

	for (I=0; I<NSAMPLES; I=I+4)
	{
//		__loop_pipelining_on__(7,2,0);
		int secondIndex = I + 1;
		int thirdIndex = I + 2;
		int fourthIndex = I + 3;

		for (SI=0; SI<NSIGMAS; SI++)
		{
			__loop_pipelining_on__(15,2,0);
			int fetchIndex0 = I + Offset[SI]; 
			int fetchIndex1 = secondIndex + Offset[SI];
			int fetchIndex2 = thirdIndex + Offset[SI];
			int fetchIndex3 = fourthIndex + Offset[SI];

			double p00 = (dotP0[SI]*hF0[fetchIndex0]);
			double p10 = (dotP1[SI]*hF1[fetchIndex0]);
			double p20 = (dotP2[SI]*hF2[fetchIndex0]);
			double p30 = (dotP3[SI]*hF3[fetchIndex0]);
			double p40 = (dotP4[SI]*hF4[fetchIndex0]);
			double p50 = (dotP5[SI]*hF5[fetchIndex0]);
			double diff0 = (inputData[I] - 
					((p00+p10) + (p20+p30) + (p40 + p50)));
			double x0 = (diff0*diff0);


			double p01 = (dotP0[SI]*hF0[fetchIndex1]);
			double p11 = (dotP1[SI]*hF1[fetchIndex1]);
			double p21 = (dotP2[SI]*hF2[fetchIndex1]);
			double p31 = (dotP3[SI]*hF3[fetchIndex1]);
			double p41 = (dotP4[SI]*hF4[fetchIndex1]);
			double p51 = (dotP5[SI]*hF5[fetchIndex1]);
			double diff1 = (inputData[secondIndex] - 
					((p01+p11) + (p21+p31) + (p41 + p51)));
			double x1 = (diff1*diff1);
			

			double p02 = (dotP0[SI]*hF0[fetchIndex2]);
			double p12 = (dotP1[SI]*hF1[fetchIndex2]);
			double p22 = (dotP2[SI]*hF2[fetchIndex2]);
			double p32 = (dotP3[SI]*hF3[fetchIndex2]);
			double p42 = (dotP4[SI]*hF4[fetchIndex2]);
			double p52 = (dotP5[SI]*hF5[fetchIndex2]);
			double diff2 = (inputData[thirdIndex] - 
					((p02+p12) + (p22+p32) + (p42 + p52)));
			double x2 = (diff2*diff2);


			double p03 = (dotP0[SI]*hF0[fetchIndex3]);
			double p13 = (dotP1[SI]*hF1[fetchIndex3]);
			double p23 = (dotP2[SI]*hF2[fetchIndex3]);
			double p33 = (dotP3[SI]*hF3[fetchIndex3]);
			double p43 = (dotP4[SI]*hF4[fetchIndex3]);
			double p53 = (dotP5[SI]*hF5[fetchIndex3]);
			double diff3 = (inputData[fourthIndex] - 
					((p03+p13) + (p23+p33) + (p43 + p53)));
			double x3 = (diff3*diff3);


			err[SI] += (x0 + x1) + (x2 + x3);
		}
	}
	for (SI=0; SI<NSIGMAS; SI++)
	{
#ifdef SW
		fprintf(stdout,"HW: Error for %d-th sigma is %f.\n", SI, err[SI]);
#endif
		if(err[SI] <  best_mse)
		{
			best_mse = err[SI];
			best_sigma_index = SI;
		}
	}
}

void bestFit()
{
	initHF();

	while(1)
	{
	
//		uint32_t start_time = getClockTime();
		// read in the samples one by one.
		// for each sample, simultaneously calculate
		// the incremental inner product across all the hF polynomials.
		RxAndComputeInnerProducts();

		// At this point you have the projections.  Calculate
		// MSE for each projection..
		computeMSE();

		// At this point, we have the best SI.
		double SI = best_sigma_index;
		write_float64("out0_data", SI);
//		write_float64("out1_data", dotP0[0]);

		int I;
			write_float64("out1_data", dotP0[best_sigma_index]);
			write_float64("out1_data", dotP1[best_sigma_index]);
			write_float64("out1_data", dotP2[best_sigma_index]);
			write_float64("out1_data", dotP3[best_sigma_index]);
			write_float64("out1_data", dotP4[best_sigma_index]);
			write_float64("out1_data", dotP5[best_sigma_index]);
//		}
//		uint32_t end_time = getClockTime();

//		write_uint32("elapsed_time_pipe", (end_time - start_time));

		initFit();

	}
}
