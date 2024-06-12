/*
 * applications.c
 *
 *  Created on: Jun 9, 2024
 *      Author: dawid.podziewski
 */
#include "applications.h"
#include "pca9685.h"

static void clearTextLine(char *arr, size_t size) {
	memset(arr, ' ', size - 1);
	arr[size - 2] = '\r';
	arr[size - 1] = '\0';
	HAL_UART_Transmit(&huart3, (uint8_t*) arr, strlen(arr), UART_DELAY);
}

static void UART_printMsg(char *msg) {
	static char message[60];
	clearTextLine(message, sizeof(message));
	strncpy(message, msg, sizeof(message) - 1);
	message[sizeof(message) - 1] = '\0';
	HAL_UART_Transmit(&huart3, (uint8_t*) message, strlen(message), UART_DELAY);
}

static void I2C_errorHandler(HAL_StatusTypeDef status) {
	if (status != HAL_OK) {
		switch (status) {
		case HAL_ERROR:
			UART_printMsg("I2C Error: HAL_ERROR\r\n");
			break;
		case HAL_BUSY:
			UART_printMsg("I2C Error: HAL_BUSY\r\n");
			break;
		case HAL_TIMEOUT:
			UART_printMsg("I2C Error: HAL_TIMEOUT\r\n");
			break;
		default:
			UART_printMsg("I2C Error: Unknown error\r\n");
			break;
		}
	}
}

static uint8_t chooseLed() {
	UART_printMsg("Enter the LED number (0 - 15):\r");
	set_rxCompleteFlag(false);
	while (!get_rxCompleteFlag())
		continue;
	uint8_t NumberLed = atoi(get_rxBuffer());
	return NumberLed;
}

static bool checkLedNumber(uint8_t LedNumber) {
	if (LedNumber >= 0 && LedNumber <= 15) {
		UART_printMsg("Selected led.\r");
		return true;
	} else {
		UART_printMsg("Invalid led number. Enter a number between 0 and 15.\r");
		return false;
	}
}

static uint8_t enterDutyCycle() {
	UART_printMsg("Set led duty cycle  0 - 100\r");
	set_rxCompleteFlag(false);
	while (!get_rxCompleteFlag())
		continue;
	uint8_t dutyCycle = atoi(get_rxBuffer());
	return dutyCycle;
}

static bool checkDutyCycle(uint8_t dutyCycle) {
	if (dutyCycle >= 0 && dutyCycle <= 100) {
		UART_printMsg("PWM set to specified duty cycle.\r");
		return true;
	} else {
		UART_printMsg("Invalid duty cycle. Enter a value between 0 and 100.\r");
		return false;
	}
}

static uint16_t enterLedsFrequency() {
	UART_printMsg("Set leds frequency  24 - 1526hz\r");
	set_rxCompleteFlag(false);
	while (!get_rxCompleteFlag())
		continue;
	uint16_t ledsFrequency = atoi(get_rxBuffer());
	return ledsFrequency;
}

static bool checkLedsFrequency(uint16_t LedsFrequency) {
	if (LedsFrequency >= 24 && LedsFrequency <= 1526) {
		return true;
	} else {
		return false;
	}
}

void operationsHandler(void) {
	if (get_rxCompleteFlag()) {

		switch ((atoi(get_rxBuffer()))) {

		case 1:
			PCA9685_enableAllLEDs();
			UART_printMsg("All LEDs have been turned on.\r");
			break;
		case 2:
			PCA9685_disableAllLEDs();
			UART_printMsg("All LEDs have been turned off.\r");
			break;
		case 3:
			uint8_t LedNumber = chooseLed();
			uint8_t dutyCycle = enterDutyCycle();
			if (checkDutyCycle(dutyCycle) && checkLedNumber(LedNumber)) {
				PCA9685_setPWM(LedNumber, 0, dutyCycle);
				UART_printMsg("PWM has been changed.\r");
			}
			break;
		case 4:
			PCA9685_sleep(&hi2c1);
			UART_printMsg("PCA9685 in sleeping mode.\r");
			break;
		case 5:
			PCA9685_wakeUp(&hi2c1);
			UART_printMsg("PCA9685 active.\r");
			break;
		case 6:
			uint16_t freq = enterLedsFrequency();
			if (PCA9685_get_sleepMode() && (checkLedsFrequency(freq))) {
				PCA9685_setLedsFrequency(freq);
				UART_printMsg("Leds frequency have been set.\r");
			} else
				UART_printMsg("Not possible to change frequency.\r");
			break;
		default:
			UART_printMsg("Invalid choice.\r");
			break;
		}
		I2C_errorHandler(PCA9685_get_I2C_status());
		set_rxCompleteFlag(false);
	}
}

void displayMenu() {
	static char message[] = "***PCA9685 MENU***\n\r"
			"1. Turn on leds\n\r"
			"2. Turn off 1eds\n\r"
			"3. Set one led duty cycle\n\r"
			"4. Sleep mode on\n\r"
			"5. Sleep mode off\n\r"
			"6. Set frequency\n\r";
	HAL_UART_Transmit(&huart3, (uint8_t*) message, strlen(message), UART_DELAY);
}
