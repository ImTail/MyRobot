#ifndef __MY_SPI
#define __MY_SPI
void MySPI_Init(void);
void MySPI_W_SS(uint8_t data);
void MySPI_Start(void);
void MySPI_End(void);
uint8_t MySPI_SwapByte(uint8_t ByteSend);
#endif
