/*
 * stm32f4xx_it.c
 *
 *  Created on: Jul 8, 2024
 *      Author: podziewski
 */
#include "main.h"
#include "stm32f4xx_it.h"
#include <string.h>
#include <stdbool.h>

#define SWITCH_DOWN     GPIO_PIN_11
#define SWITCH_MIDDLE   GPIO_PIN_15
#define SWITCH_UP       GPIO_PIN_9
#define SWITCH_RIGHT    GPIO_PIN_6
#define SWITCH_LEFT     GPIO_PIN_8

#define INIT_PSC        11998
#define CLOCK_MHZ       24000000
#define KHZ_5           5000
#define TOTAL_CHANNEL   4

/* Private variables ---------------------------------------------------------*/
static unsigned int freq = 10;
static bool isEnabled = false;
static const uint16_t Tim_channels[] = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4 }; // Zmienna statyczna
static size_t channel_index = 0;
static TIM_HandleTypeDef htim4;

/* Private function prototypes -----------------------------------------------*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void adjust_CCR(int direction);

/* Interrupt Handlers --------------------------------------------------------*/

void init_timer(TIM_HandleTypeDef *htim)
{
    htim4 = *htim;

    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}

void EXTI9_5_IRQHandler(void)
{
    if (__HAL_GPIO_EXTI_GET_FLAG(SWITCH_RIGHT)) {
        adjust_CCR(1);
    } else if (__HAL_GPIO_EXTI_GET_FLAG(SWITCH_LEFT)) {
        adjust_CCR(-1);
    } else if (__HAL_GPIO_EXTI_GET_FLAG(SWITCH_UP)) {
        freq += KHZ_5;
        TIM4->PSC = CLOCK_MHZ / ((TIM4->ARR + 1) * freq);
    }

    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}

void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(SWITCH_DOWN);
    HAL_GPIO_EXTI_IRQHandler(SWITCH_MIDDLE);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}

void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim4);
}

/* Private functions ---------------------------------------------------------*/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
    case SWITCH_DOWN:
        if (freq > KHZ_5)
            freq -= KHZ_5;
        if (freq < 200)
        {
            TIM4->PSC = INIT_PSC;
        }
        else
        {
            TIM4->PSC = CLOCK_MHZ / ((TIM4->ARR + 1) * freq);
        }
        break;
    case SWITCH_MIDDLE:
        if (!isEnabled)
        {
            isEnabled = true;
            channel_index = 0;
            for (int i = 0; i < TOTAL_CHANNEL; i++)
            {
                HAL_TIM_PWM_Start(&htim4, Tim_channels[i]);
            }
        }
        else if (channel_index == TOTAL_CHANNEL - 1)
        {
            for (int i = 0; i < TOTAL_CHANNEL; i++)
            {
                HAL_TIM_PWM_Stop(&htim4, Tim_channels[i]);
            }
            isEnabled = false;
        }
        else
        {
            channel_index++;
        }
        break;
    }
}

void adjust_CCR(int direction)
{
    if (isEnabled)
    {
        switch (channel_index)
        {
        case 0:
            if (direction > 0 && TIM4->CCR1 < TIM4->ARR)
                TIM4->CCR1++;
            else if (direction < 0 && TIM4->CCR1 > 0)
                TIM4->CCR1--;
            break;
        case 1:
            if (direction > 0 && TIM4->CCR2 < TIM4->ARR)
                TIM4->CCR2++;
            else if (direction < 0 && TIM4->CCR2 > 0)
                TIM4->CCR2--;
            break;
        case 2:
            if (direction > 0 && TIM4->CCR3 < TIM4->ARR)
                TIM4->CCR3++;
            else if (direction < 0 && TIM4->CCR3 > 0)
                TIM4->CCR3--;
            break;
        case 3:
            if (direction > 0 && TIM4->CCR4 < TIM4->ARR)
                TIM4->CCR4++;
            else if (direction < 0 && TIM4->CCR4 > 0)
                TIM4->CCR4--;
            break;
        }
    }
}
