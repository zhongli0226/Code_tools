#include "cw32f030_uart.h"
#include "uart.h"
//==================================================================================================
//  实现功能: 高级定时器
//  函数说明: ATIMER_init 
//  函数备注: 
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//================================================================================================== 
void UART1_Iint(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	

	RCC_APBPeriphClk_Enable2(DEBUG_USART_CLK, ENABLE);
	RCC_AHBPeriphClk_Enable(DEBUG_USART_GPIO_CLK, ENABLE);  

    PA08_AFx_UART1TXD();
	PA09_AFx_UART1RXD(); 
	
    GPIO_InitStructure.Pins = DEBUG_USART_TX_GPIO_PIN; //PA8
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pins = DEBUG_USART_RX_GPIO_PIN; //PA9
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
		

    USART_InitStructure.USART_BaudRate = DEBUG_USART_BaudRate;
    USART_InitStructure.USART_Over = USART_Over_16;
    USART_InitStructure.USART_Source = USART_Source_PCLK;
    USART_InitStructure.USART_UclkFreq = DEBUG_USART_UclkFreq;
    USART_InitStructure.USART_StartBit = USART_StartBit_FE;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(DEBUG_USARTx, &USART_InitStructure); 
	
	
    //使能UARTx RC中断
    USART_ITConfig(DEBUG_USARTx, USART_IT_RC, ENABLE);
    //优先级，无优先级分组
    NVIC_SetPriority(UART1_IRQn, 0);
    //UARTx中断使能
    NVIC_EnableIRQ(UART1_IRQn);

}

/**
 * @brief Retargets the C library printf function to the USART.
 *
 */
#ifdef __GNUC__
int _write(int fd, char* ptr, int len)
{
    for(int i = 0; i < len; i++)
    {
        USART_SendData_8bit(DEBUG_USARTx, ptr[i]);
        while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);
    }
    return len;
}
#endif

#ifdef __CC_ARM
int fputc(int ch, FILE *f)
{
    USART_SendData_8bit(DEBUG_USARTx, ((uint8_t*))ch);
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);
    return ch;
}
#endif