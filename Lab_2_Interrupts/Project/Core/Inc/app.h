/*
 * app.h
 *
 *  Created on: Jul 8, 2024
 *      Author: podzi
 */

#ifndef INC_APP_H_
#define INC_APP_H_

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

void operations();
void EXTI_Callback(uint16_t GPIO_Pin);

#endif /* INC_APP_H_ */
