#ifndef __MYUSART_H
#define __MYUSART_H
extern uint8_t RX_Data_3;
extern uint8_t RX_Data_1[20];

void MyUSART_Init(void);
void MyUSART1_W_Byte(uint8_t data);
void MyUSART3_W_Byte(uint8_t data);
uint8_t MyUSART1_G_Flag(void);
uint8_t MyUSART3_G_Flag(void);
#endif
