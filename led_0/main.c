#include "main.h"

#define SWITCH_DELAY 500000

int main(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable peripheral clock for LEDs port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  /* Init LEDs */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  /* Turn all the leds off */
  GPIO_SetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15);

  int i;  
  int k;

  while (1)
  {
    /* Switch the LED on */
    for(k = 0; k < 4; k++)
    {
    	GPIO_ResetBits(GPIOD, GPIO_Pin_12<<k);
    	for (i = 0; i < SWITCH_DELAY; i++);
    }
    /* Switch the LED off */
    for(k = 0; k < 4; k++)
    {
    	GPIO_SetBits(GPIOD, GPIO_Pin_12<<k);
    	for (i = 0; i < SWITCH_DELAY; i++);
    }
  }
}
