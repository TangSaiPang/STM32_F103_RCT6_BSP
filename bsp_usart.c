#include "bsp_usart.h"

UART_HandleTypeDef huart1;

#ifdef __GNUC__
/*
 * With GCC, small printf (option LD Linker -> Libraries -> Small printf
 * set to 'Yes') calls __io_putchar()
 * */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

void DEBUG_USART_Config(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 128000;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
	GPIO_InitTypeDef USART_GPIO_Init;

	if(uartHandle->Instance==USART1)
	{
		/* USART1 clock enable */
		__HAL_RCC_USART1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();

		/**USART1 GPIO Configuration
		    PA9     ------> USART1_TX
		    PA10     ------> USART1_RX
		    */

		USART_GPIO_Init.Pin = USART1_TX_Pin;
		USART_GPIO_Init.Mode = GPIO_MODE_AF_PP;
		USART_GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(USART1_TX_GPIO_Port, &USART_GPIO_Init);

		USART_GPIO_Init.Pin = USART1_RX_Pin;
		USART_GPIO_Init.Mode = GPIO_MODE_INPUT;
		USART_GPIO_Init.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(USART1_RX_GPIO_Port, &USART_GPIO_Init);

		/* USART1 interrupt Init */
	    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	    HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
}

PUTCHAR_PROTOTYPE
{
	while((USART1 -> SR & 0X40) == 0);
	USART1 -> DR = (uint8_t)ch;
	return ch;
}
