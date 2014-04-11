#include <vhdlCStubs.h>
void QRSFilt(uint8_t initialize)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call QRSFilt ");
append_int(buffer,1); ADD_SPACE__(buffer);
append_uint8_t(buffer,initialize); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
uint8_t blsCheck()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call blsCheck ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,8); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint8_t ret_val__ = get_uint8_t(buffer,&ss);
return(ret_val__);
}
uint32_t circUpdateDet(uint32_t ptr,uint32_t size)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call circUpdateDet ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_uint32_t(buffer,ptr); ADD_SPACE__(buffer);
append_uint32_t(buffer,size); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint32_t ret_val__ = get_uint32_t(buffer,&ss);
return(ret_val__);
}
uint32_t circUpdateFilt(uint32_t ptr,uint32_t size)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call circUpdateFilt ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_uint32_t(buffer,ptr); ADD_SPACE__(buffer);
append_uint32_t(buffer,size); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint32_t ret_val__ = get_uint32_t(buffer,&ss);
return(ret_val__);
}
uint32_t ddCalc(uint32_t datum)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call ddCalc ");
append_int(buffer,1); ADD_SPACE__(buffer);
append_uint32_t(buffer,datum); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint32_t ret_val__ = get_uint32_t(buffer,&ss);
return(ret_val__);
}
void deriv()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call deriv ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
uint64_t divideSigned(uint64_t dividend,uint32_t divisor)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call divideSigned ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_uint64_t(buffer,dividend); ADD_SPACE__(buffer);
append_uint32_t(buffer,divisor); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,64); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint64_t ret_val__ = get_uint64_t(buffer,&ss);
return(ret_val__);
}
uint64_t divideUnsigned(uint64_t dividend,uint32_t divisor)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call divideUnsigned ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_uint64_t(buffer,dividend); ADD_SPACE__(buffer);
append_uint32_t(buffer,divisor); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,64); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint64_t ret_val__ = get_uint64_t(buffer,&ss);
return(ret_val__);
}
void filterx_xoptx_xo_storage_initializer_()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call filterx_xoptx_xo_storage_initializer_ ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
void global_storage_initializer_()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call global_storage_initializer_ ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
void hpFilt()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call hpFilt ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
void initDet()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call initDet ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
void initFilt()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call initFilt ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
void lpFilt()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call lpFilt ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
uint32_t meanCalc(uint32_t* buffer)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call meanCalc ");
append_int(buffer,1); ADD_SPACE__(buffer);
append_uint32_t(buffer,(uint32_t) buffer); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint32_t ret_val__ = get_uint32_t(buffer,&ss);
return(ret_val__);
}
uint8_t mem_load__(uint32_t address)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call mem_load__ ");
append_int(buffer,1); ADD_SPACE__(buffer);
append_uint32_t(buffer,address); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,8); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint8_t data = get_uint8_t(buffer,&ss);
return(data);
}
void mem_store__(uint32_t address,uint8_t data)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call mem_store__ ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_uint32_t(buffer,address); ADD_SPACE__(buffer);
append_uint8_t(buffer,data); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
void mvWin()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call mvWin ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
void noiseUpdate(uint32_t noiseVal)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call noiseUpdate ");
append_int(buffer,1); ADD_SPACE__(buffer);
append_uint32_t(buffer,noiseVal); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
uint32_t peak()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call peak ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint32_t ret_val__ = get_uint32_t(buffer,&ss);
return(ret_val__);
}
void ptrFuncx_xoptx_xo_storage_initializer_()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call ptrFuncx_xoptx_xo_storage_initializer_ ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
void qrsDet()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call qrsDet ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
void qrsDetx_xoptx_xo_storage_initializer_()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call qrsDetx_xoptx_xo_storage_initializer_ ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
void qrsUpdate(uint32_t qrsVal,uint32_t rrVal)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call qrsUpdate ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_uint32_t(buffer,qrsVal); ADD_SPACE__(buffer);
append_uint32_t(buffer,rrVal); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
uint32_t revUpdateDet(uint32_t ptr,uint32_t max)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call revUpdateDet ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_uint32_t(buffer,ptr); ADD_SPACE__(buffer);
append_uint32_t(buffer,max); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint32_t ret_val__ = get_uint32_t(buffer,&ss);
return(ret_val__);
}
uint32_t threshCalc(uint32_t qmean,uint32_t nmean)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call threshCalc ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_uint32_t(buffer,qmean); ADD_SPACE__(buffer);
append_uint32_t(buffer,nmean); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint32_t ret_val__ = get_uint32_t(buffer,&ss);
return(ret_val__);
}
void threshold_initializer_in_qrsDetx_xoptx_xo()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call threshold_initializer_in_qrsDetx_xoptx_xo ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
