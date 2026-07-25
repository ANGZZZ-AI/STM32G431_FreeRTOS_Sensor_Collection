#include "led.h"

static const uint16_t led_pins[8] = {
    GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11,
    GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15
};

void LED_UpdateLatch(void)
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

void LED_AllOff(void)
{
    uint16_t all = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                  |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_WritePin(GPIOC, all, GPIO_PIN_SET);
    LED_UpdateLatch();
}

void LED_On(uint8_t index)
{
    if(index < 8)
        HAL_GPIO_WritePin(GPIOC, led_pins[index], GPIO_PIN_RESET);
    LED_UpdateLatch();
}

void LED_Off(uint8_t index)
{
    if(index < 8)
        HAL_GPIO_WritePin(GPIOC, led_pins[index], GPIO_PIN_SET);
    LED_UpdateLatch();
}

void LED_Flash(uint8_t index)
{
    LED_On(index);
    HAL_Delay(200);
    LED_Off(index);
}

/* Keep old names for compatibility */
void LED_Set(uint8_t index)   { LED_On(index); }
void LED_Clear(uint8_t index) { LED_Off(index); }
