#ifndef fp_ops___h___
#define fp_ops___h___
#include <stdint.h>
#include "app_defines.h"

double fp64_mul_for_ecg(double X, double Y);
double fp64_addsub_for_ecg(double X, double Y, uint8_t sub_flag);
double fp64_add_for_ecg(double X, double Y);
double fp64_sub_for_ecg(double X, double Y);

float fp32_mul_for_ecg(float X, float Y);
float fp32_addsub_for_ecg(float X, float Y, uint8_t sub_flag);
float fp32_add_for_ecg(float X, float Y);
float fp32_sub_for_ecg(float X, float Y);

#endif
