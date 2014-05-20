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
float hF0[MEM_SIZE];
float hF1[MEM_SIZE];
float hF2[MEM_SIZE];
float hF3[MEM_SIZE];
float hF4[MEM_SIZE];
float hF5[MEM_SIZE];
float hFall[MEM_SIZE*6];

float dotP0[NSIGMAS];
float dotP1[NSIGMAS];
float dotP2[NSIGMAS];
float dotP3[NSIGMAS];
float dotP4[NSIGMAS];
float dotP5[NSIGMAS];

float inputData[NSAMPLES];
float err[NSIGMAS]; 
int Offset[NSIGMAS]; 

void bestFit();
