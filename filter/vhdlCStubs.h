#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <Pipes.h>
#include <SocketLib.h>
void QRSFilt(uint8_t initialize);
uint8_t blsCheck();
uint32_t circUpdateDet(uint32_t ptr,uint32_t size);
uint32_t circUpdateFilt(uint32_t ptr,uint32_t size);
uint32_t ddCalc(uint32_t datum);
void deriv();
uint64_t divideSigned(uint64_t dividend,uint32_t divisor);
uint64_t divideUnsigned(uint64_t dividend,uint32_t divisor);
void filterx_xoptx_xo_storage_initializer_();
void global_storage_initializer_();
void hpFilt();
void initDet();
void initFilt();
void lpFilt();
uint32_t meanCalc(uint32_t* buffer);
uint8_t mem_load__(uint32_t address);
void mem_store__(uint32_t address,uint8_t data);
void mvWin();
void noiseUpdate(uint32_t noiseVal);
uint32_t peak();
void ptrFuncx_xoptx_xo_storage_initializer_();
void qrsDet();
void qrsDetx_xoptx_xo_storage_initializer_();
void qrsUpdate(uint32_t qrsVal,uint32_t rrVal);
uint32_t revUpdateDet(uint32_t ptr,uint32_t max);
uint32_t threshCalc(uint32_t qmean,uint32_t nmean);
void threshold_initializer_in_qrsDetx_xoptx_xo();
