/*
 * applications.h
 *
 *  Created on: Jun 9, 2024
 *      Author: dawid.podziewski
 */

#ifndef INC_APPLICATIONS_H_
#define INC_APPLICATIONS_H_

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"

#define UART_DELAY 200
extern UART_HandleTypeDef huart3;

void displayMenu();
void operationsHandler();

#endif /* INC_APPLICATIONS_H_ */
