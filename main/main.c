#include "main.h"

static GPIO_InitTypeDef  GPIO_InitStruct;

int main(void)
{

    HAL_Init();

    /* -1- Enable GPIOA Clock (to be able to program the configuration registers) */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /* -2- Configure PA05 IO in output push-pull mode to drive external LED */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 

    while(1)
    {       
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

        HAL_Delay( 500 );
    }

    
    return 0;
}