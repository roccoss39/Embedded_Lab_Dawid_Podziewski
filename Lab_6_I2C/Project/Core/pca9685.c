#include <pca9685.h>
#include "stdbool.h"
#include <stdio.h> // Include for snprintf
#include <string.h> // Include for strlen
#include <math.h> // Include for roundf

uint8_t TxBuffer[2];
bool sleepMode = false;
bool UART_Received = false;

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart3;

void clearTextLine(char *arr, size_t size) {
	memset(arr, ' ', size - 1);
	arr[size - 2] = '\r';
	arr[size - 1] = '\0';
	HAL_UART_Transmit(&huart3, (uint8_t*) arr, strlen(arr), 100);

}
void printMsg(char *msg, uint8_t choice) {
	static char message[50];
	clearTextLine(message, sizeof(message));

	if ((sleepMode) && (choice != '8') && (choice != '9')) {
		static char sleepInfo[50] =
				"Sleeping mode on. Press 7 to active PCA9685.\r";
		HAL_UART_Transmit(&huart3, (uint8_t*) sleepInfo, strlen(sleepInfo),
				100);

	} else {
		strncpy(message, msg, sizeof(message) - 1);
		message[sizeof(message) - 1] = '\0';
		HAL_UART_Transmit(&huart3, (uint8_t*) message, strlen(message), 100);

	}
}

void PCA9685_enableAllLEDs() {
	TxBuffer[0] = PCA9685_ALL_LED_L;
	TxBuffer[1] = 0xFF;
	HAL_I2C_Master_Transmit(&hi2c1, PCA9685_ADDRESS, TxBuffer, 2, 1000);

	TxBuffer[0] = PCA9685_ALL_LED_H;
	TxBuffer[1] = 0x0F;
	HAL_I2C_Master_Transmit(&hi2c1, PCA9685_ADDRESS, TxBuffer, 2, 1000);

}

void PCA9685_disableAllLEDs() {
	TxBuffer[0] = PCA9685_ALL_LED_L;
	TxBuffer[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, PCA9685_ADDRESS, TxBuffer, 2, 1000);

	TxBuffer[0] = PCA9685_ALL_LED_H;
	TxBuffer[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, PCA9685_ADDRESS, TxBuffer, 2, 1000);

}

void PCA9685_setLedsFrequency(float frequency) {

	uint8_t prescaler = (uint8_t) roundf(25000000.0f / (4096 * frequency)) - 1;
	uint8_t TxBuffer[2];
	TxBuffer[0] = PCA9685_PRESCALER;
	TxBuffer[1] = prescaler;
	HAL_I2C_Master_Transmit(&hi2c1, PCA9685_ADDRESS, TxBuffer, 2, 1000);
}

void PCA9685_sleep(I2C_HandleTypeDef *hi2c) {
	uint8_t mode1_reg;
	HAL_I2C_Master_Receive(&hi2c1, PCA9685_ADDRESS << 1, &mode1_reg, 1, 1000);
	TxBuffer[0] = 0x00;
	TxBuffer[1] = PCA9685_SLEEP;
	HAL_I2C_Master_Transmit(&hi2c1, PCA9685_ADDRESS, TxBuffer, 2, 1000);

	sleepMode = true;
}

void PCA9685_wakeUp(I2C_HandleTypeDef *hi2c) {
	TxBuffer[0] = PCA9685_MODE1;
	TxBuffer[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, PCA9685_ADDRESS, TxBuffer, 2, 1000);

	HAL_Delay(1);

	TxBuffer[0] = PCA9685_MODE1;
	TxBuffer[1] = 0x80;
	HAL_I2C_Master_Transmit(&hi2c1, PCA9685_ADDRESS, TxBuffer, 2, 1000);

	sleepMode = false;
}

void PCA9685_setLedsPWM(unsigned timeOff) {
	TxBuffer[0] = PCA9685_ALL_LED_L;
	TxBuffer[1] = timeOff;
	HAL_I2C_Master_Transmit(&hi2c1, PCA9685_ADDRESS, TxBuffer, 2, 1000);

	TxBuffer[0] = PCA9685_ALL_LED_H;
	TxBuffer[1] = timeOff >> 8u;
	HAL_I2C_Master_Transmit(&hi2c1, PCA9685_ADDRESS, TxBuffer, 2, 1000);
}
