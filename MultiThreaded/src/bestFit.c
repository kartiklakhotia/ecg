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


void initHF()
{
	int M = MEM_SIZE;
	int idx;
	int offset1, offset2, offset3, offset4, offset5;
	offset1 = M;
	offset2 = 2*M;
	offset3 = 3*M;
	offset4 = 4*M;
	offset5 = 5*M;
	for(idx = 0; idx < NSIGMAS; idx++)
	{
		__loop_pipelining_on__(8,2,0);
		dotP0[idx]  = 0;
		dotP1[idx]  = 0;
		dotP2[idx]  = 0;
		dotP3[idx]  = 0;
		dotP4[idx]  = 0;
		dotP5[idx]  = 0;
	}

	for (idx = 0; idx < 6*M; idx++){
		hFall[idx] = read_float64("in1_data");
	}

	for(idx = 0; idx < M; idx++)
	{
		__loop_pipelining_on__(8,2,0);
		hF0[idx] = hFall[idx];
		hF1[idx] = hFall[offset1+idx];
		hF2[idx] = hFall[offset2+idx];	
		hF3[idx] = hFall[offset3+idx];
		hF4[idx] = hFall[offset4+idx];
		hF5[idx] = hFall[offset5+idx];
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
	}
}


// given the next input, the hf polynomial
// array and the dotp array, update the dotp
// arrays for all sigma positions with 
// the inner product computation.
// 
// 
inline void  __InnerProduct__(int SI) {

/// later define them as global variables and see if resource utilization goes down and timing goes up.  
	double p00, p10, p20, p30, p40, p50, dataSample0;
	double p01, p11, p21, p31, p41, p51, dataSample1;
	double p02, p12, p22, p32, p42, p52, dataSample2;
	double p03, p13, p23, p33, p43, p53, dataSample3;
	int offset = SI*NSAMPLES;
	int base;
	int I,index0,index1, index2, index3;
	for (I=0; I<NSAMPLES; I=I+4)
	{
		__loop_pipelining_on__(15,2,0);
		base = offset + I;
		index0 = base;	
		dataSample0 = inputData[I];
		index1 = base + 1;	
		dataSample1 = inputData[I+1];
		index2 = base + 2;	
		dataSample2 = inputData[I+2];
		index3 = base + 3;	
		dataSample3 = inputData[I+3];




		p00 = dataSample0*hF0[index0];
		p01 = dataSample1*hF0[index1];
		p02 = dataSample2*hF0[index2];
		p03 = dataSample3*hF0[index3];
		dotP0[SI] += (p00 + p01) + (p02 + p03);
	
		p10 = dataSample0*hF1[index0];
		p11 = dataSample1*hF1[index1];
		p12 = dataSample2*hF1[index2];
		p13 = dataSample3*hF1[index3];
		dotP1[SI] += (p10 + p11) + (p12 + p13);

		p20 = dataSample0*hF2[index0];
		p21 = dataSample1*hF2[index1];
		p22 = dataSample2*hF2[index2];
		p23 = dataSample3*hF2[index3];
		dotP2[SI] += (p20 + p21) + (p22 + p23);

		p30 = dataSample0*hF3[index0];
		p31 = dataSample1*hF3[index1];
		p32 = dataSample2*hF3[index2];
		p33 = dataSample3*hF3[index3];
		dotP3[SI] += (p30 + p31) + (p32 + p33);

		p40 = dataSample0*hF4[index0];
		p41 = dataSample1*hF4[index1];
		p42 = dataSample2*hF4[index2];
		p43 = dataSample3*hF4[index3];
		dotP4[SI] += (p40 + p41) + (p42 + p43);

		p50 = dataSample0*hF5[index0];
		p51 = dataSample1*hF5[index1];
		p52 = dataSample2*hF5[index2];
		p53 = dataSample3*hF5[index3];
		dotP5[SI] += (p50 + p51) + (p52 + p53);
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
		double x = read_float64("in0_data");
		inputData[I] = x;

		// note: the sample interval will about 
		// 1ms.  Assuming a clock of 100MHz, this
		// means that the inner-product loop below
		// should finish in 100K cycles.  not likely
		// to be critical.
	}
	write_uint32("startSignalPipe", 1);
	for (I=0; I<NSIGMAS; I++){
		__InnerProduct__(I);
		write_uint32("startSignalPipe", 1);
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
	double best_mse;
	int best_sigma_index ;
	uint32_t start;
	while (1){
		start = read_uint32("startSignalPipe");
		best_mse = 1.0e+20;
		best_sigma_index = -1;
	
		for(SI = 0; SI < NSIGMAS; SI++)
		{
			double err = 0.0;
			start = read_uint32("startSignalPipe");
	
			double s0 = dotP0[SI];
			double s1 = dotP1[SI];
			double s2 = dotP2[SI];
			double s3 = dotP3[SI];
			double s4 = dotP4[SI];
			double s5 = dotP5[SI];
	
			int Offset = (SI * NSAMPLES);
	
			for(I = 0; I < NSAMPLES; I=I+4)
			{
				__loop_pipelining_on__(15,2,0);
				int base = I + Offset;
				int fetchIndex0 = base;
				int fetchIndex1 = (base + 1);
				int fetchIndex2 = (base + 2);
				int fetchIndex3 = (base + 3);
	
				double p00 = (s0*hF0[fetchIndex0]);
				double p10 = (s1*hF1[fetchIndex0]);
				double sum_00_10 = p00 + p10;
		
	
				double p20 = (s2*hF2[fetchIndex0]);
				double p30 = (s3*hF3[fetchIndex0]);
				double sum_20_30 = p20 + p30;
	
	
				double p40 = (s4*hF4[fetchIndex0]);
				double p50 = (s5*hF5[fetchIndex0]);
				double sum_40_50 = p40 + p50;
	
	
				double diff0 = ((inputData[I] - sum_00_10) - (sum_20_30 + sum_40_50)); 
	
				double x0 = (diff0*diff0);
	
	
				double p01 = (s0*hF0[fetchIndex1]);
				double p11 = (s1*hF1[fetchIndex1]);
				double sum_01_11 = p01 + p11;
	
	
				double p21 = (s2*hF2[fetchIndex1]);
				double p31 = (s3*hF3[fetchIndex1]);
				double sum_21_31 = p21 + p31;
	
	
				double p41 = (s4*hF4[fetchIndex1]);
				double p51 = (s5*hF5[fetchIndex1]);
				double sum_41_51 = p41 + p51;
	
				double diff1 = ((inputData[I+1] - sum_01_11) - (sum_21_31 + sum_41_51));
	
				double x1 = (diff1*diff1);
				
				double p02 = (s0*hF0[fetchIndex2]);
				double p12 = (s1*hF1[fetchIndex2]);
				double sum_02_12 = p02 + p12;
	
	
				double p22 = (s2*hF2[fetchIndex2]);
				double p32 = (s3*hF3[fetchIndex2]);
				double sum_22_32 = p22 + p32;
	
	
				double p42 = (s4*hF4[fetchIndex2]);
				double p52 = (s5*hF5[fetchIndex2]);
				double sum_42_52 = p42 + p52;
	
				double diff2 = ((inputData[I+2] - sum_02_12) - (sum_22_32 + sum_42_52));
	
				double x2 = (diff2*diff2);
	
				double p03 = (s0*hF0[fetchIndex3]);
				double p13 = (s1*hF1[fetchIndex3]);
				double sum_03_13 = p03 + p13;
	
	
				double p23 = (s2*hF2[fetchIndex3]);
				double p33 = (s3*hF3[fetchIndex3]);
				double sum_23_33 = p23 + p33;
	
	
				double p43 = (s4*hF4[fetchIndex3]);
				double p53 = (s5*hF5[fetchIndex3]);
				double sum_43_53 = p43 + p53;
	
				double diff3 = ((inputData[I+3] - sum_03_13) - (sum_23_33 + sum_43_53));
	
				double x3 = (diff3*diff3);
	
	
	
	
				err += (x0 + x1) + (x2 + x3);
			}
	
#ifdef SW
			fprintf(stdout,"HW: Error for %d-th sigma is %f.\n", SI, err);
#endif
	//		best_mse = err;
			//write_uint32("logger_pipe", SI);
	
			if(err <  best_mse)
			{
				best_mse = err;
				best_sigma_index = SI;
			}
		}
	
		write_uint32("doneSignalPipe", best_sigma_index); 
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
//		computeMSE();
		uint32_t best_sigma_index = read_uint32("doneSignalPipe");
		// At this point, we have the best SI.
		double SI = best_sigma_index;
		write_float64("out0_data", SI);

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
