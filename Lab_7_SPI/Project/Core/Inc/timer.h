/*
 * timer.h
 *
 *  Created on: Jun 6, 2024
 *      Author: dawid.podziewski
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>


bool get_transmitFlag();

void set_transmitFlag(uint8_t value);


#endif /* INC_TIMER_H_ */
