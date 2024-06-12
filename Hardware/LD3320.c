#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "delay.h"

#include "OLED.h"
#include "LD3320.h"


#define CLK_IN   			22.1184	/* user need modify this value according to clock in */
#define LD_PLL_11			(uint8_t)((CLK_IN/2.0)-1)
#define LD_PLL_MP3_19		0x0f
#define LD_PLL_MP3_1B		0x18
#define LD_PLL_MP3_1D   	(uint8_t)(((90.0*((LD_PLL_11)+1))/(CLK_IN))-1)

#define LD_PLL_ASR_19 		(uint8_t)(CLK_IN*32.0/(LD_PLL_11+1) - 0.51)
#define LD_PLL_ASR_1B 		0x48
#define LD_PLL_ASR_1D 		0x1f

#define LD3320_PORT	GPIOA
#define LD3320_Pin_RST GPIO_Pin_3

extern uint8_t ASRStatus;


void LD3320_Init(void)
{
	MySPI_Init();
	//配置复位控制引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_Init_Structrue;
	GPIO_Init_Structrue.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init_Structrue.GPIO_Pin=LD3320_Pin_RST;
	GPIO_Init_Structrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(LD3320_PORT,&GPIO_Init_Structrue);
}

uint8_t LD3320_RunOnce(void)
{
	uint8_t flag = 0;
	for (uint8_t k = 0 ; k<5 ; k++)	//尝试５次
	{	
		LD3320_CommonInit();
		LD3320_ASRInit();
		Delay_ms(20);
		if (LD3320_AddTip()==0)		//添加失败
		{
			LD3320_RST();
			Delay_ms(20);
			continue;
		}
		if (LD3320_Start()==0)		//开始失败
		{
			LD3320_RST();
			Delay_ms(20);
			continue;
		}
		flag = 1;
		break;
	}
	return flag;
}

void LD3320_W_RST(uint8_t val)
{
	GPIO_WriteBit(LD3320_PORT,LD3320_Pin_RST,(BitAction)val);
}

void LD3320_W_Reg(uint8_t addr,uint8_t val)
{
	MySPI_Start();
	MySPI_SwapByte(0x04);		//写指令
	MySPI_SwapByte(addr);		//寄存器地址
	MySPI_SwapByte(val);		//写入寄存器的值
	MySPI_End();
}

uint8_t LD3320_R_Reg(uint8_t addr)
{
	uint8_t res;
	MySPI_Start();
	MySPI_SwapByte(0x05);		//读指令
	MySPI_SwapByte(addr);		//寄存器地址
	res=MySPI_SwapByte(0x00);	//传入任意字节，读取字节
	MySPI_End();
	return res;
}

//根据手册推荐复位
void LD3320_RST(void)
{
	//RST引脚发送低电平
	LD3320_W_RST(1);
	Delay_ms(10);
	LD3320_W_RST(0);
	Delay_ms(10);
	LD3320_W_RST(1);
	//对片选拉低拉高，激活内部DSP
	Delay_ms(10);
	MySPI_W_SS(0);
	Delay_ms(10);
	MySPI_W_SS(1);
	Delay_ms(10);
}

//第一步通用初始化
void LD3320_CommonInit(void)
{
	LD3320_R_Reg(0x06);//读状态,只有３ 5位有用到
	LD3320_W_Reg(0x17,0x35);//软复位
	Delay_ms(10);
	LD3320_R_Reg(0x06);
	LD3320_W_Reg(0x89,0x03);//初始化写入03,播放mp3不一样
	Delay_ms(5);
	LD3320_W_Reg(0xCF,0x43);//省电模式初始化写入43,MPS和ASR初始化不一样
	Delay_ms(5);
	LD3320_W_Reg(0xCB,0x02);//写入候补寄存器４
	
	LD3320_W_Reg(0x11,LD_PLL_11);//时钟频率1
	
	LD3320_W_Reg(0x1E,0x00);//ADC控制初始化
	LD3320_W_Reg(0x19,LD_PLL_ASR_19);//时钟频率2
	LD3320_W_Reg(0x1B,LD_PLL_ASR_1B);//时钟频率3
	LD3320_W_Reg(0x1D,LD_PLL_ASR_1D);//时钟频率4
	
	LD3320_W_Reg(0xCD,0x04);//允许DSP休眠
	LD3320_W_Reg(0x17,0x4C);//休眠DSP
	Delay_ms(5);
	LD3320_W_Reg(0xB9,0x00);//识别语句长度，初始化写入00
	LD3320_W_Reg(0xCF,0X4F);//内部省电模式，ASR初始化写入
	LD3320_W_Reg(0x6F,0xFF);//初始化FF
}

//语音识别初始化
void LD3320_ASRInit(void)
{
	LD3320_W_Reg(0xBD,0x00);//启动ASR模块
	LD3320_W_Reg(0x17,0x48);//激活DSP
	Delay_ms(10);
	LD3320_W_Reg(0x3C,0x80);//FIFO_EXT 下限低 8 位
	LD3320_W_Reg(0x3E,0x07);//FIFO_EXT 下限高 8 位
	LD3320_W_Reg(0x38,0xff);//FIFO_EXT 上限低 8 位
	LD3320_W_Reg(0x3A,0x07);//FIFO_EXT 上限高 8 位
	LD3320_W_Reg(0x40,0x00);//FIFO_EXT MCU 水线低 8 位
	LD3320_W_Reg(0x42,0x08);//FIFO_EXT MCU 水线高 8 位
	LD3320_W_Reg(0x44,0x00);//FIFO_EXT DSP 水线低 8 位 
	LD3320_W_Reg(0x46,0x08);//FIFO_EXT DSP 水线高 8 位
	Delay_ms(1);
}

//B2寄存器是0x21时，表示ASR空闲。 返回1空闲,返回0繁忙
uint8_t LD3320_CheckAsrBusy(void)
{
	uint8_t flag = 0;
	if (LD3320_R_Reg(0xB2) == 0x21)
	{
		flag = 1;
	}
	return flag;
}

//添加识别条目
uint8_t LD3320_AddTip(void)
{
	const char mess[5][20] = {"ba er liu liu","zai jian","kai deng", 
		"guan deng", "ni shi shui"};
	const uint8_t messCode[5] = {1,2,3,4,5};
	
	uint8_t flag = 1;//表示添加成功
	for (uint8_t k = 0 ; k < 5 ; k++)
	{
		if (LD3320_CheckAsrBusy() == 0)	//若某一条　条目　录入检测到繁忙，则退出循环，函数返回０，表示添加失败。
		{
			flag=0;
			break;
		}
		
		uint8_t len;					//用于记录字符串长度
		LD3320_W_Reg(0XC1,messCode[k]);	//C1寄存器存储接下来录入的语句ID，当前录入第K个ID
		LD3320_W_Reg(0XC3,0X00);		//添加语句时写入00
		LD3320_W_Reg(0X08,0X04);		//清除某寄存器内容
		Delay_ms(1);
		LD3320_W_Reg(0X08,0X00);		//清除后需要输入00
		Delay_ms(1);
		for (len = 0 ; len < 20 ; len++)//遍历当前语句的每一个字节，并依次写入0x05数据口,记录字符串长度。
		{
			if (mess[k][len] == '\0')
			{
				break;
			} else
			{
			LD3320_W_Reg(0x05, mess[k][len]);
			}
		}
		
			LD3320_W_Reg(0XB9,len);
			LD3320_W_Reg(0XB2,0XFF);	//重置ASR忙闲寄存器
			LD3320_W_Reg(0X37,0X04);	//0x37中写入0x04，通知添加一条新语句。
		Delay_ms(20);
	}
	return flag;
}

//启动ASR模块
uint8_t LD3320_Start(void)
{
	LD3320_W_Reg(0x35,0x40);//adc增益00 ~ 7F	,越大识别越敏感
	LD3320_W_Reg(0x1C,0x09);//ADC开关，保留命令字
	LD3320_W_Reg(0xBD,0x20);//初始化控制寄存器，保留命令字
	LD3320_W_Reg(0X08,0X01);//清除某寄存器内容
	Delay_ms(1);
	LD3320_W_Reg(0X08,0X00);//清除后需要输入00
	Delay_ms(1);
	if (LD3320_CheckAsrBusy() == 0)	//忙碌，函数退出，返回０
	{
		return 0;
	}
	LD3320_W_Reg(0xB2, 0xFF);//重置ASR忙闲寄存器
	LD3320_W_Reg(0x37,0x06);//0x37中写入0x06，通知开始识别。
	Delay_ms(5);
	LD3320_W_Reg(0x1C, 0x0B);//启动麦克风输入增益
    LD3320_W_Reg(0x29,0x10);//中断允许，第４位，同步中断允许
	LD3320_W_Reg(0xBD,0x00);//写入00启动为ASR模块
	return 1;
}

uint8_t LD3320_G_Res(void)
{
	return LD3320_R_Reg(0xC5);
}

uint8_t LD3320_G_ResCount(void)
{
	return LD3320_R_Reg(0xBA);
}

uint8_t LD3320_G_HaveResFlag(void)
{
	if (LD3320_R_Reg(0xBF) == 0x35 && LD3320_R_Reg(0xB2) == 0x21)//BF=35代表有结果产生,读取后值会改变，只要没有下一次结果，不会进入循环。
	{
		return 1;
	} else 
	{
		return 0;
	}
}
