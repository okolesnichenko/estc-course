#include "main.h"

#define DELAY 500000

int main(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  int delay = DELAY;
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
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure); 
  /* Turn all the leds off */
  GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);
  //GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1);
  
  int k,i;
  uint8_t change_0,change_1;

  while (1)
  {  
    change_0 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0);
    change_1 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1);
    // If pressed GPIO_Pin_0 -> right direction
    // If pressed Pin_1 -> left direction
    if(!change_0)
    {
	delay +=100000;
    }
    if(!change_1)
    {
	if(delay > 100000)
        {
	   delay -=100000;
        }
	else
	{
	   delay = 500000;
	}
    }
    for(k = 0; k < 3; k++)
    {
	GPIO_ResetBits(GPIOA, GPIO_Pin_8<<k);
	for(i = 0; i < delay; i++);
    }
    for(k = 0; k < 3; k++)
    {
	GPIO_SetBits(GPIOA, GPIO_Pin_8<<k);
	for(i = 0; i < delay; i++);
    }
 }
}
