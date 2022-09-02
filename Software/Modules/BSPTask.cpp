#include "BSPTask.h"
#include "tim.h"

void RGB_Task(void const * argument)
{
	int8_t  RGB_ADD[3]={1,1,1};
	uint16_t RGB[3]={333,0,999};
	for(;;)
	{
		for(uint8_t i=0;i<3;i++){
		RGB[i] += RGB_ADD[i];
		if(RGB[i]==0)RGB_ADD[i]*=-1;
		else if(RGB[i]==1000)RGB_ADD[i]*=-1;
		}
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_4,RGB[0]);
		__HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,RGB[1]);
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3,RGB[2]);
		
		osDelay(2);
	}
}
	
