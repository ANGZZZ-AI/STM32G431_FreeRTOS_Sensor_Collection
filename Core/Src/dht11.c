#include "dht11.h"
#include "FreeRTOS.h"
#include "task.h"

/* Microsecond delay using DWT cycle counter */
static void delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000);
    while ((DWT->CYCCNT - start) < ticks);
}

/* Millisecond delay using DWT (not HAL_Delay to avoid task switching) */
static void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++) {
        delay_us(1000);
    }
}

/* Set GPIO as output */
static void DHT11_SetOutput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

/* Set GPIO as input with pull-up */
static void DHT11_SetInput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

/* Read GPIO pin state */
static uint8_t DHT11_ReadPin(void)
{
    return (uint8_t)HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin);
}

/* Write GPIO pin state */
static void DHT11_WritePin(uint8_t state)
{
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void DHT11_Init(void)
{
    /* Enable DWT cycle counter for precise timing */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    /* Enable GPIOB clock */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Set PB10 as output high initially */
    DHT11_SetOutput();
    DHT11_WritePin(1);

    /* Wait for DHT11 to stabilize (>1s after power-on) */
    delay_ms(2000);
}

HAL_StatusTypeDef DHT11_ReadData(uint8_t *temp, uint8_t *humi)
{
    uint8_t data[5] = {0};
    uint8_t i, j;
    uint32_t timeout;
    UBaseType_t saved_irq_state;

    /* Enter critical section - disable interrupts for timing-sensitive operation */
    saved_irq_state = taskENTER_CRITICAL_FROM_ISR();

    /* Step 1: Send start signal - pull low for 18ms+ */
    DHT11_SetOutput();
    DHT11_WritePin(0);
    delay_ms(18);  /* 18ms pull low */
    DHT11_WritePin(1);
    delay_us(40);  /* 20-40us pull high */

    /* Step 2: Switch to input and wait for DHT11 response */
    DHT11_SetInput();

    /* Wait for DHT11 to pull low (response signal, ~80us) */
    timeout = 0;
    while (DHT11_ReadPin() == GPIO_PIN_SET) {
        if (++timeout > 1000) {
            taskEXIT_CRITICAL_FROM_ISR(saved_irq_state);
            return HAL_ERROR;
        }
        delay_us(1);
    }

    /* Wait for DHT11 low pulse (~80us) */
    timeout = 0;
    while (DHT11_ReadPin() == GPIO_PIN_RESET) {
        if (++timeout > 1000) {
            taskEXIT_CRITICAL_FROM_ISR(saved_irq_state);
            return HAL_ERROR;
        }
        delay_us(1);
    }

    /* Wait for DHT11 high pulse (~80us) */
    timeout = 0;
    while (DHT11_ReadPin() == GPIO_PIN_SET) {
        if (++timeout > 1000) {
            taskEXIT_CRITICAL_FROM_ISR(saved_irq_state);
            return HAL_ERROR;
        }
        delay_us(1);
    }

    /* Step 3: Read 40 bits (5 bytes) */
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 8; j++) {
            /* Wait for low period (~50us) */
            timeout = 0;
            while (DHT11_ReadPin() == GPIO_PIN_RESET) {
                if (++timeout > 1000) {
                    taskEXIT_CRITICAL_FROM_ISR(saved_irq_state);
                    return HAL_ERROR;
                }
                delay_us(1);
            }

            /* Wait 40us then check: if still high, it's a '1' bit */
            delay_us(40);

            if (DHT11_ReadPin() == GPIO_PIN_SET) {
                data[i] |= (1 << (7 - j));
                /* Wait for high pulse to end */
                timeout = 0;
                while (DHT11_ReadPin() == GPIO_PIN_SET) {
                    if (++timeout > 1000) {
                        taskEXIT_CRITICAL_FROM_ISR(saved_irq_state);
                        return HAL_ERROR;
                    }
                    delay_us(1);
                }
            }
        }
    }

    /* Exit critical section */
    taskEXIT_CRITICAL_FROM_ISR(saved_irq_state);

    /* Step 4: Verify checksum */
    if (data[4] != ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
        return HAL_ERROR;
    }

    /* Step 5: Extract temperature and humidity */
    *humi = data[0];  /* Humidity integer part */
    *temp = data[2];  /* Temperature integer part */

    return HAL_OK;
}
