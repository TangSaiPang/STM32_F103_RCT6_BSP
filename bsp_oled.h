/*
 * bsp_oled.h
 *
 *  Created on: May 28, 2023
 *      Author: Saipa
 */

#ifndef OLED_BSP_OLED_H_
#define OLED_BSP_OLED_H_
#include "stm32f1xx.h"

extern I2C_HandleTypeDef hi2c1;

#define OLED_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_SCL_Pin                GPIO_PIN_6
#define OLED_GPIO_Port              GPIOB
#define OLED_SDA_Pin                GPIO_PIN_7

#define OLED_SCL_ON					HAL_GPIO_WritePin(OLED_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_SET)
#define OLED_SCL_OFF				HAL_GPIO_WritePin(OLED_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_RESET)
#define OLED_SCL_TOGGLE     		HAL_GPIO_TogglePin(OLED_GPIO_Port, OLED_SCL_Pin)
#define OLED_SDA_ON					HAL_GPIO_WritePin(OLED_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_SET)
#define OLED_SDA_OFF				HAL_GPIO_WritePin(OLED_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_RESET)
#define OLED_SDA_TOGGLE     		HAL_GPIO_TogglePin(OLED_GPIO_Port, OLED_SDA_Pin)

#define OLED_CMD  0	// To write command
#define OLED_DATA 1	// To write data

void OLED_SHowfloat(uint8_t x,uint8_t y,float num,uint8_t a,uint8_t b,uint8_t size);
void WriteCmd(void);
void OLED_WR_CMD(uint8_t cmd);
void OLED_WR_DATA(uint8_t data);
void OLED_Clear(void);
void OLED_Display_ON(void);
void OLED_Display_OFF(void);
void OLED_SET_Pos(uint8_t x, uint8_t y);
void OLED_Refresh(void);
uint32_t OLED_Pow(uint8_t m,uint8_t n);
void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_Init(void);
void IIC1_GPIO_Init(void);

#endif /* OLED_BSP_OLED_H_ */
