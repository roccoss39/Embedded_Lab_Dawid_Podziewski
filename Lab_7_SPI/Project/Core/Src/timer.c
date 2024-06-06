/*
 * timer.c
 *
 *  Created on: Jun 6, 2024
 *      Author: dawid.podziewski
 */

#include "timer.h"

static bool transmitFlag = 0;

bool get_transmitFlag()
{
	return transmitFlag;
}

void set_transmitFlag(uint8_t value) {
	transmitFlag = value;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM1) {
	}
	set_transmitFlag(true);
	//transmitCoordinates(&huart3, myData);
}
