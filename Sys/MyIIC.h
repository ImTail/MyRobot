#ifndef __MYIIC_H
#define __MYIIC_H
void MyIIC_Init(void);
void MyIIC_Start(void);
void MyIIC_End(void);
void MyIIC_W_Byte(uint8_t data);
uint8_t MyIIC_R_ACK(void);
#endif
