/*
 * application.c
 *
 *  Created on: Jul 8, 2024
 *      Author: podzi
 */
#include "application.h"

#define NUM_LEDS 4
#define HAL_DELAY 200
#define HAL_DELAY_LONG 500

const uint16_t led_pins[] = {GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};

void led_operation() {
	for (int i = 0; i < NUM_LEDS; i++) {
		HAL_GPIO_WritePin(GPIOD, led_pins[i], GPIO_PIN_SET);
		HAL_Delay(HAL_DELAY_LONG);
		HAL_GPIO_WritePin(GPIOD, led_pins[i], GPIO_PIN_RESET);
	}

	for (int j = 0; j < NUM_LEDS; j++) {
		for (int i = 0; i < 4; i++) {
			HAL_GPIO_WritePin(GPIOD, led_pins[i], GPIO_PIN_SET);
		}
		HAL_Delay(HAL_DELAY);
		for (int i = 0; i < 4; i++) {
			HAL_GPIO_WritePin(GPIOD, led_pins[i], GPIO_PIN_RESET);
		}
		HAL_Delay(HAL_DELAY);
	}
}
