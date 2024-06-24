# 这是一个语音助手的小项目

1. ## **项目简介**

   - ### 项目名称

     暂无

   - ### 项目背景和目标

     意在设计一个能语音交互的助手

     可以根据需要播报当前时间，各种传感器得出的数据等。

     也有可视化的LED屏幕显示相关图像数据。

   - ### 关键特性

     语音交互

2. ## **硬件设计**

   - ### 主控芯片

     STM32F103C8T6

   - ### 所用模块和硬件连接

     OLED (SSD1306主控) - I2C - 语音识别

     ​	B8：SCK

     ​	B9:	SDA

     LD3320 (STC51主控) - SPI - 屏幕显示

     ​	A3:LD3320-RST

     ​	A4:	SC

     ​	A5:	SCK

     ​	A6: 	MI

     ​	A7:	MO

     ​	LD3320-WR:LD3320-GND

     SYN6288 - USART -　TTS

     ​	B10:SYN6288-RX

     ​	B11:SYN6288-TX

     EPS8266-USASRT-网络连接

     ​	3.3V:ESP8266-RST	ESP8266-EN	ESP8266-RST	ESP8266-I00	ESP8266-I02

     ​	A9:ESP8266-RX

     ​	A10:ESP8266-TX

     STLINK-SWD-程序烧录

     ​	SWD:STLINK-SWDIO

     ​	SWDSCL:STLINK-SWDSCL

3. ## **软件设计**

   - ### 软件架构概览

     系统延时模块

     IIC驱动

     OLED驱动

     动画模块

     LED驱动

     KEY驱动

     SPI驱动

     语音驱动

     网络驱动

   - ### 主要模块和功能介绍

     LD3320语音识别

     SYN6288文字转语音

     OLED显示

     ESP8266网络通讯

   - ### 使用的编程语言和开发环境

     KEIL5:C语言

4. ## **功能实现**

   - ### 各个功能模块的详细实现方法

     见源码

   - ### 使用的库函数和外部库

     string.h

     stm32f10x.h