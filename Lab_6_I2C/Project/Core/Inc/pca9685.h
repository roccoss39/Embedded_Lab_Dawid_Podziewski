/*
 * pca9685.h
 *
 *  Created on: May 23, 2024
 *      Author: dawid.podziewski
 */

#ifndef INC_PCA9685_H_
#define INC_PCA9685_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <math.h> // Include for roundf

#define PCA9685_ADDRESS (0x80)
#define PCA9685_SLEEP 0x10
#define PCA9685_PRESCALER 0xFE
#define PCA9685_MODE1 0x00

#define PCA9685_ALL_LED_L 0xFC
#define PCA9685_ALL_LED_H 0xFD
#define PCA9685_LED0_OFF_L 0x08
#define PCA9685_LED0_OFF_H 0x09

#define I2C_TRANSMIT_DELAY 1000

extern I2C_HandleTypeDef hi2c1;

// Function Prototypes
void PCA9685_enableAllLEDs();
void PCA9685_disableAllLEDs();
void PCA9685_setLedsFrequency(uint16_t frequency);
void PCA9685_wakeUp(I2C_HandleTypeDef *hi2c);
void PCA9685_sleep(I2C_HandleTypeDef *hi2c);
bool PCA9685_get_sleepMode();
void PCA9685_setPWM(uint8_t channel, uint16_t on, uint16_t off);
uint8_t PCA9685_get_I2C_status();


#endif /* INC_PCA9685_H_ */
