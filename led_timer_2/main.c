#include "main.h"

#define PERIOD 600 
#define PRESCALER 42000
#define COLOR_LENGTH 9

uint32_t led_colors[COLOR_LENGTH] = {GPIO_Pin_8, GPIO_Pin_8 | GPIO_Pin_9, GPIO_Pin_8 | GPIO_Pin_10, 
				GPIO_Pin_9, GPIO_Pin_9 | GPIO_Pin_8, GPIO_Pin_9 | GPIO_Pin_10,
				GPIO_Pin_10, GPIO_Pin_10 | GPIO_Pin_8, GPIO_Pin_10 | GPIO_Pin_9};

int main(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  
  /*Init timer */
  TIM_TimeBaseInitTypeDef tim_struct;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  tim_struct.TIM_Period = PERIOD - 1;
  tim_struct.TIM_Prescaler = PRESCALER - 1;
  tim_struct.TIM_ClockDivision = 0;
  tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &tim_struct);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
  /* Enable peripheral clock for LEDs port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	
  /* Init LEDs */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Init buttons */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure); 
  /* Turn all the leds off */
  GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);
  int k; 
  uint8_t change_0,change_1;
  char direct = 0;
  while (1)
  {
    change_0 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0);
    change_1 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1);
    // If pressed GPIO_Pin_0 -> right direction
    // If pressed Pin_1 -> left direction
    if(!change_0)
    {
	direct = 0;
    }
    if(!change_1)
    {
	direct = 1;
    }
    if(direct)
    {
    	for(k = 0; k < COLOR_LENGTH; k++)
    	{
		GPIO_ResetBits(GPIOA, led_colors[k]);
		while(TIM_GetCounter(TIM2));
    	}
    	for(k = 0; k < COLOR_LENGTH; k++)
    	{
		GPIO_SetBits(GPIOA, led_colors[k]);
		while(TIM_GetCounter(TIM2));
    	}
     }
     else
     {
        for(k = COLOR_LENGTH; k > 0; k--)
    	{
		GPIO_ResetBits(GPIOA, led_colors[k]);
		while(TIM_GetCounter(TIM2));
    	}
    	for(k = COLOR_LENGTH; k > 0; k--)
    	{
		GPIO_SetBits(GPIOA, led_colors[k]);
		while(TIM_GetCounter(TIM2));
    	}
     }
 }
}
