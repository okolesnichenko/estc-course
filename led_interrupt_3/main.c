#include "main.h"

#define SWITCH_DELAY 400
#define PERIOD 600 
#define PRESCALER 42000
static int k;

void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {    	
	//TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);	
 	GPIO_ResetBits(GPIOA, GPIO_Pin_8<<k);
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	
  }
}

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
  /* Init Interrupt config */
    NVIC_InitTypeDef nvic_struct;
  nvic_struct.NVIC_IRQChannel = TIM2_IRQn;
  nvic_struct.NVIC_IRQChannelPreemptionPriority = 0;
  nvic_struct.NVIC_IRQChannelSubPriority = 1;
  nvic_struct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic_struct);
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
  k = 0;
  while (1)
  {
    //int i;
    uint8_t a,b;
    a = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0);
    b = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1);
    if(!a)
    {
	k++;
	if(k > 2) k = 0;
    }
    if(!b)
    {
	k--;
	if(k < 0) k = 2;
    }
    //GPIO_ResetBits(GPIOA, GPIO_Pin_8<<k);
    //while(TIM_GetCounter(TIM2));

    //GPIO_SetBits(GPIOA, GPIO_Pin_8<<k);
    //while(TIM_GetCounter(TIM2));
 }
}
