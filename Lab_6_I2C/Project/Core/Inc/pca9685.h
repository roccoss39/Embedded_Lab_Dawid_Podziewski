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

#define PCA9685_ADDRESS (0x80)
#define PCA9685_SLEEP 0x10
#define PCA9685_ALL_LED_L 0xFC
#define PCA9685_ALL_LED_H 0xFD
#define PCA9685_PRESCALER 0xFE
#define PCA9685_MODE1 0x00

extern uint8_t TxBuffer[2];
extern bool sleepMode;
extern bool UART_Received;

// Function Prototypes
void clearTextLine(char *arr, size_t size);
void printMsg(char *msg, uint8_t choice);
void PCA9685_enableAllLEDs();
void PCA9685_disableAllLEDs();
void PCA9685_setLedsPWM(unsigned timeOff);
void PCA9685_setLedsFrequency(float frequency);
void PCA9685_wakeUp(I2C_HandleTypeDef *hi2c);
void PCA9685_sleep(I2C_HandleTypeDef *hi2c);


#endif /* INC_PCA9685_H_ */
