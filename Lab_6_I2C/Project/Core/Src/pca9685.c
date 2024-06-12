#include <pca9685.h>

static bool sleepMode = false;
static HAL_StatusTypeDef I2C_status = HAL_OK;

static uint8_t PCA9685_SetRegister(uint8_t registerAddress, uint8_t data) {
    uint8_t TxBuffer[2];
    TxBuffer[0] = registerAddress;
    TxBuffer[1] = data;

    HAL_StatusTypeDef I2C_status = HAL_I2C_Master_Transmit(&hi2c1, PCA9685_ADDRESS, TxBuffer, 2, I2C_TRANSMIT_DELAY);
    return I2C_status;
}

void PCA9685_enableAllLEDs() {
	PCA9685_SetRegister(PCA9685_ALL_LED_L, 0xFF);
	PCA9685_SetRegister(PCA9685_ALL_LED_H, 0x0F);
}

void PCA9685_disableAllLEDs() {
	PCA9685_SetRegister(PCA9685_ALL_LED_L, 0x00);
	PCA9685_SetRegister(PCA9685_ALL_LED_H, 0x00);
}

void PCA9685_setLedsFrequency(uint16_t frequency) {
	uint8_t prescaler = (uint8_t) roundf(25000000.0f / ((float)4096 * frequency)) - 1;
	PCA9685_SetRegister(PCA9685_PRESCALER, prescaler);
}

void PCA9685_sleep(I2C_HandleTypeDef *hi2c) {
	uint8_t mode1_reg;
	HAL_I2C_Master_Receive(&hi2c1, PCA9685_ADDRESS << 1, &mode1_reg, 1, 1000);
	PCA9685_SetRegister(PCA9685_MODE1, PCA9685_SLEEP);

	sleepMode = true;
}

void PCA9685_wakeUp(I2C_HandleTypeDef *hi2c) {
	PCA9685_SetRegister(PCA9685_MODE1, 0x00);
	HAL_Delay(1);
	PCA9685_SetRegister(PCA9685_MODE1, 0x80);

	sleepMode = false;
}

void PCA9685_setPWM(uint8_t channel, uint16_t on, uint16_t dutyCycle) {
		dutyCycle = (uint16_t) (4095 * ((float) dutyCycle / 100));
		PCA9685_SetRegister(PCA9685_LED0_OFF_L + 4 * channel, dutyCycle);
		PCA9685_SetRegister(PCA9685_LED0_OFF_H + 4 * channel, (dutyCycle >> 8u));
}

bool PCA9685_get_sleepMode() {
	return sleepMode;
}

uint8_t PCA9685_get_I2C_status() {
	return I2C_status;
}
