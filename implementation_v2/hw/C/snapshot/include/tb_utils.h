#ifndef __tb_utils_h____
#define __tb_utils_h____

uint8_t tbGetUint8();
void	tbSendUint8(uint8_t X);

uint16_t tbGetUint16();
void	 tbSendUint16(uint16_t X);

uint32_t tbGetUint32();
void	 tbSendUint32(uint32_t X);

float 	tbGetFloat();
void	tbSendFloat(float X);

uint64_t tbGetUint64();
void	 tbSendUint64(uint64_t X);

double 	 tbGetDouble();
void	 tbSendDouble(double X);



#endif
