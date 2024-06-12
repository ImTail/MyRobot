#ifndef __SYN6288_H
#define __SYN6288_H
void SYN6288_Init(void);
void SYN6288_W_Data(uint8_t command,uint8_t commandVal,char* stringData,uint16_t stringLen);
uint8_t SYN6288_R_Data(void);
void SYN6288_Speech(char stringData[]);
#endif
