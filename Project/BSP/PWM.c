#include "PWM.h"

#define PWM_TIM_ARR 20000

/*===| 初始化PWM TIM1 TIM8 |===*/
void PWM_Init(void)
{
	HAL_TIM_PWM_Start(PWM_TIM, PWM_1_TIM_CHANNAL);
	HAL_TIM_PWM_Start(PWM_TIM, PWM_2_TIM_CHANNAL);
	HAL_TIM_PWM_Start(PWM_TIM, PWM_3_TIM_CHANNAL);
	HAL_TIM_PWM_Start(PWM_TIM, PWM_4_TIM_CHANNAL);
}


/*===| 设置各个通道的占空比 |===*/
void PWM_SetEmpty(uint8_t Channel, float Empty)
{
	switch(Channel)
	{
		case 1: __HAL_TIM_SetCompare(PWM_TIM, PWM_1_TIM_CHANNAL, Empty * PWM_TIM_ARR); break;
		case 2: __HAL_TIM_SetCompare(PWM_TIM, PWM_2_TIM_CHANNAL, Empty * PWM_TIM_ARR); break;
		case 3: __HAL_TIM_SetCompare(PWM_TIM, PWM_3_TIM_CHANNAL, Empty * PWM_TIM_ARR); break;
		case 4: __HAL_TIM_SetCompare(PWM_TIM, PWM_4_TIM_CHANNAL, Empty * PWM_TIM_ARR); break;
	}
}

