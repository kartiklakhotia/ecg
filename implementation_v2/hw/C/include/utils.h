#ifndef __utils_h____
#define __utils_h____

uint8_t getUint8();
void	sendUint8(uint8_t X);

uint16_t getUint16();
void	 sendUint16(uint16_t X);

uint32_t getUint32();
void	 sendUint32(uint32_t X);

float 	getFloat();
void	sendFloat(float X);

uint64_t getUint64();
void	 sendUint64(uint64_t X);

double 	 getDouble();
void	 sendDouble(double X);

void sendFloatToHermiteFitter(float X);
float  getFloatForHermiteFitter();
void sendDoubleToHermiteFitter(double X);
double  getDoubleForHermiteFitter();


#endif
