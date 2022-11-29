#ifndef _hermite_h___
#define _hermite_h___
#include "app_defines.h"

void fillHermitePolynomialsInConfig();
REAL nFactor(int N);
REAL basisScaleFactor(int N, REAL sigma, REAL x);
REAL hermitePolynomial(int N, REAL x);
REAL hermiteBasisFunction(int N, REAL sigma, REAL x);
void normalizedHermiteBasisFunction(int N, REAL sigma, REAL* nhf);

#endif
