#ifndef __OLED_H
#define __OLED_H
void OLED_W_Command(uint8_t command);
void OLED_W_Data(uint8_t data);
void OLED_W_Datas(uint8_t* datas,uint16_t count);
void OLED_Init(void);
void OLED_SetCursor(uint8_t x,uint8_t page);
void OLED_ClearAll(void);
void OLED_ShowChar(uint8_t column,uint8_t line,char ch,uint8_t cover);
void OLED_ShowString(uint8_t column,uint8_t line,char* str,uint8_t len,uint8_t cover);
void OLED_Show_Img(uint8_t x,uint8_t y,uint8_t* datas,uint8_t width,uint8_t height,uint8_t cover);
void OLED_ShowChinese(uint8_t x,uint8_t y,char* chineseChar);
void OLED_ShowHexNum(uint8_t column,uint8_t line,uint8_t num,uint8_t cover);
#endif
