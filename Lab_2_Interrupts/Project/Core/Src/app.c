/*
 * app.c
 *
 *  Created on: Jul 8, 2024
 *      Author: podzi
 */
#include "app.h"

#define LED_1 GPIO_PIN_12
#define LED_2 GPIO_PIN_13
#define LED_3 GPIO_PIN_14
#define LED_4 GPIO_PIN_15

#define MAX_SCHEME 2
#define MIN_SCHEME 0
#define MIN_DELAY_TIME 50000
#define MAX_DELAY_TIME 4000000
#define DELAY_INCREMENT 50000
#define DELAY_DEFAULT 1000000
#define DELAY_SHORT 500

static volatile int8_t currentScheme = 0;
static const uint16_t arrLeds[] = { LED_1, LED_2, LED_3, LED_4 };
static const uint8_t numOfLEDs = sizeof(arrLeds) / sizeof(arrLeds[0]);
static uint8_t lastLed = 0;
static unsigned int delayTime = 1000000;
static bool isActivated = false;
static bool buttonPressed = false;

static void myDelay()
{
	for (uint32_t i = 0; i < delayTime; i++)
	{
		continue;
	}
}

static void turnOffBlinking()
{
	for (uint8_t i = 0; i < numOfLEDs; ++i)
	{
		HAL_GPIO_WritePin(GPIOD, arrLeds[i], GPIO_PIN_RESET);
	}
}

static void blinkingRight()
{
	for (uint8_t i = lastLed; i < numOfLEDs; ++i)
	{

		HAL_GPIO_WritePin(GPIOD, arrLeds[i], GPIO_PIN_SET);

		if (!buttonPressed && isActivated)
			myDelay();

		HAL_GPIO_WritePin(GPIOD, arrLeds[i], GPIO_PIN_RESET);

		if (buttonPressed || !isActivated)
		{
			buttonPressed = false;
			break;
		}
		lastLed = (i == numOfLEDs - 1) ? 0 : i + 1;
	}
}

static void blinkingLeft()
{
	for (uint8_t i = lastLed; i < numOfLEDs; ++i)
	{

		HAL_GPIO_WritePin(GPIOD, arrLeds[i], GPIO_PIN_SET);

		if (!buttonPressed && isActivated)
			myDelay();

		HAL_GPIO_WritePin(GPIOD, arrLeds[i], GPIO_PIN_RESET);

		if (buttonPressed || !isActivated)
		{
			buttonPressed = false;
			break;
		}
		lastLed = (i == numOfLEDs - 1) ? 0 : i + 1;
	}
}

static void blinking()
{
	for (uint8_t i = 0; i < numOfLEDs; i++)
	{
		HAL_GPIO_WritePin(GPIOD, arrLeds[i], GPIO_PIN_SET);
	}

	if (!buttonPressed && isActivated)
		myDelay();
	else
		buttonPressed = false;

	for (uint8_t i = 0; i < numOfLEDs; i++)
	{
		HAL_GPIO_WritePin(GPIOD, arrLeds[i], GPIO_PIN_RESET);
	}

	if (!buttonPressed && isActivated)
		myDelay();

	if (buttonPressed || !isActivated)
		buttonPressed = false;
}

void operations()
{
	if (isActivated)
	{
		switch (currentScheme)
		{
		case 0:
			blinkingRight();
			break;
		case 1:
			blinkingLeft();
			break;
		case 2:
			blinking();
			break;
		default:
			break;
		}
	}
	else
	{
		if (buttonPressed)
		{
			turnOffBlinking();
			buttonPressed = false;
		}
		HAL_Delay(DELAY_SHORT);
	}
}

void EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin)
	{
	case GPIO_PIN_6:
		if (++currentScheme > MAX_SCHEME)
			currentScheme = MIN_SCHEME;
		buttonPressed = true;
		break;
	case GPIO_PIN_8:
		if (--currentScheme < MIN_SCHEME)
			currentScheme = MAX_SCHEME;
		buttonPressed = true;
		break;
	case GPIO_PIN_9:
		if (delayTime > MIN_DELAY_TIME)
			delayTime -= DELAY_INCREMENT;
		break;
	case GPIO_PIN_15:
		isActivated = !isActivated;
		buttonPressed = true;
		break;
	case GPIO_PIN_11:
		if (delayTime < MAX_DELAY_TIME)
			delayTime += DELAY_INCREMENT;
		break;
	default:
		break;
	}
}
