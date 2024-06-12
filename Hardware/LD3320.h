#ifndef __LD3320_H
#define __LD3320_H
void LD3320_Init(void);
uint8_t LD3320_RunOnce(void);
void LD3320_W_RST(uint8_t val);
void LD3320_W_Reg(uint8_t addr,uint8_t val);
uint8_t LD3320_R_Reg(uint8_t addr);
void LD3320_RST(void);
void LD3320_CommonInit(void);
void LD3320_ASRInit(void);
uint8_t LD3320_CheckAsrBusy(void);
uint8_t LD3320_AddTip(void);
uint8_t LD3320_Start(void);
uint8_t LD3320_G_Res(void);
uint8_t LD3320_G_ResCount(void);
uint8_t LD3320_G_HaveResFlag(void);
#endif
