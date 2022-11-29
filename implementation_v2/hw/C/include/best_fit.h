#ifndef __best_fit_h___
#define __best_fit_h___

// for now, each heartbeat is 200 samples long.
// Lets assume it is padded to 256.  Thus, 
// for each hermite basis function (for N=0,1,2,3,4,5)
// we can keep 16 values of sigma.  Should be enough?
#include "app_defines.h"

void initializeHermitePolynomials();
uint32_t findBestFit();

#endif
