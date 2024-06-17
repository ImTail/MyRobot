#ifndef __ESP8266_H
#define __ESP8266_H


void ESP8266_Init(void);
void ESP8266_RST(void);
uint8_t ESP8266_W_AT(char* atCommand,char* response,uint16_t tryCounts);

#endif
