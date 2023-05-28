/*
 * bsp_oled.c
 *
 *  Created on: May 28, 2023
 *      Author: Saipa
 */

#include "bsp_oled.h"
#include "oledfont.h"

I2C_HandleTypeDef hi2c1;

uint8_t OLED_GRAM[144][8];

uint8_t CMD_Data[] = {
		0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F,

		0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12,

		0xD8, 0x30, 0x8D, 0x14, 0xAF};      //初始化命令;

void OLED_SHowfloat(uint8_t x,uint8_t y,float num,uint8_t a,uint8_t b,uint8_t size)
{
    if(num<0)
    {
        OLED_ShowChar(x,y,'-',size);
        x+=size/2;
        num=-num;

    OLED_ShowNum(x,y,num,a,16);
    OLED_ShowChar(x+a*(size/2),y,'.',16);
    for(int i=1;i<=b;i++)
        {
            OLED_ShowNum(x+(a+i)*(size/2),y,(uint16_t)(num*10)%10,1,size);
            num*=10;
        }
    }

    else
    {
    OLED_ShowNum(x,y,num,a,16);
    OLED_ShowChar(x+a*(size/2),y,'.',16);
    for(int i=1;i<=b;i++)
        {
            OLED_ShowNum(x+(a+i)*(size/2),y,(uint16_t)(num*10)%10,1,size);
            num*=10;
        }
    }
}

void WriteCmd(void)
{
	uint8_t i = 0;
	for(i=0; i<27; i++)
	{
		HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,CMD_Data+i,1,0x100);
	}
}
//向设备写控制命令
void OLED_WR_CMD(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,&cmd,1,0x100);
}
//向设备写数据
void OLED_WR_DATA(uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&data,1,0x100);
}

//清屏
void OLED_Clear(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_CMD(0xb0+i);
		OLED_WR_CMD (0x02);
		OLED_WR_CMD (0x10);
		for(n=0;n<128;n++)
			OLED_WR_DATA(0);
	}
}
//开启OLED显示
void OLED_Display_ON(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC命令
	OLED_WR_CMD(0X14);  //DCDC ON
	OLED_WR_CMD(0XAF);  //DISPLAY ON
}
//关闭OLED显示
void OLED_Display_OFF(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC命令
	OLED_WR_CMD(0X10);  //DCDC OFF
	OLED_WR_CMD(0XAE);  //DISPLAY OFF
}
void OLED_SET_Pos(uint8_t x, uint8_t y)
{
	OLED_WR_CMD(0xb0+y);
	OLED_WR_CMD(((2+x&0xf0)>>4)|0x10);
	OLED_WR_CMD(2+x&0x0f);
}

void OLED_Refresh(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_CMD(0xb0+i);    //设置页地址（0~7）
		OLED_WR_CMD(0x00);      //设置显示位置—列低地址
		OLED_WR_CMD(0x10);      //设置显示位置—列高地址
		for(n=0;n<128;n++)
			OLED_WR_DATA(1);
	} //更新显示
}
uint32_t OLED_Pow(uint8_t m,uint8_t n)
{
	unsigned int result=1;
	while(n--)result*=m;
	return result;
}
//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2)
{
	uint8_t t,temp;
	uint8_t enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1;

		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2);
	}
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{
	unsigned char c=0,i=0;
	c=chr-' ';//得到偏移后的值
	if(x>128-1)
	{	x=0;
		y=y+2;
	}
	if(Char_Size ==16)
	{
		OLED_SET_Pos(x,y);
		for(i=0;i<8;i++)
		OLED_WR_DATA(asc2_1608[c*16+i]);
		OLED_SET_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WR_DATA(asc2_1608[c*16+i+8]);
	}
	else
	{
		OLED_SET_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WR_DATA(asc2_0806[c][i]);
	}
}

//显示一个字符号串
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
//hzk 用取模软件得出的数组
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{
	uint8_t t,adder=0;
	OLED_SET_Pos(x,y);
    for(t=0;t<16;t++)
		{
				OLED_WR_DATA(Hzk1[2*no][t]);
				adder+=1;
     }
		OLED_SET_Pos(x,y+1);
    for(t=0;t<16;t++)
			{
				OLED_WR_DATA(Hzk1[2*no+1][t]);
				adder+=1;
      }
}


//OLED的初始化
void OLED_Init(void)
{
	HAL_Delay(200);
	WriteCmd();
}

void IIC1_GPIO_Init(void)
{
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 400000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		Error_Handler();
	}
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

	GPIO_InitTypeDef OLED_GPIO_Init;
	if(i2cHandle->Instance==I2C1)
	{
		OLED_GPIO_CLK_ENABLE();
		/**I2C1 GPIO Configuration
		* PB6     ------> I2C1_SCL
		* PB7     ------> I2C1_SDA
		*/
		OLED_GPIO_Init.Pin = 	OLED_SCL_Pin | OLED_SDA_Pin;
		OLED_GPIO_Init.Mode = 	GPIO_MODE_AF_OD;
		OLED_GPIO_Init.Speed = 	GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(OLED_GPIO_Port, &OLED_GPIO_Init);

		/* I2C1 clock enable */
		__HAL_RCC_I2C1_CLK_ENABLE();


	}
}

