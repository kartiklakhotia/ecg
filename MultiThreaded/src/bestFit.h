// for now, each heartbeat is 200 samples long.
// Lets assume it is padded to 256.  Thus, 
// for each hermite basis function (for N=0,1,2,3,4,5)
// we can keep 16 values of sigma.  Should be enough?
#define NSAMPLES  144
#define NSIGMAS     10
#define MEM_SIZE  (NSAMPLES*NSIGMAS)
#define SAMPLING_INTERVAL (1.0/360.0)


// globals: the stored hermite Functions.
// for N=0 to 5 (first index)
// and for 16 values of sigma..
// (the exact values of the sigma's are
//  not important).
double hF00[MEM_SIZE];
double hF10[MEM_SIZE];
double hF20[MEM_SIZE];
double hF30[MEM_SIZE];
double hF40[MEM_SIZE];
double hF50[MEM_SIZE];

double hF01[MEM_SIZE];
double hF11[MEM_SIZE];
double hF21[MEM_SIZE];
double hF31[MEM_SIZE];
double hF41[MEM_SIZE];
double hF51[MEM_SIZE];


double dotP0[NSIGMAS];
double dotP1[NSIGMAS];
double dotP2[NSIGMAS];
double dotP3[NSIGMAS];
double dotP4[NSIGMAS];
double dotP5[NSIGMAS];

double inputData[NSAMPLES];

void bestFit();
