/*
 * dht11.h
 * DHT11 Temperature and Humidity Sensor Driver
 * Created for STM32G431 FreeRTOS Project
 */

#ifndef __DHT11_H
#define __DHT11_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/* DHT11 GPIO Configuration - User must define these in main.h */
#ifndef DHT11_GPIO_Port
#define DHT11_GPIO_Port GPIOB
#endif

#ifndef DHT11_Pin
#define DHT11_Pin GPIO_PIN_10
#endif

/**
 * @brief Read temperature and humidity from DHT11 sensor
 * @param temp Pointer to store temperature value (integer, °„C)
 * @param humi Pointer to store humidity value (integer, %)
 * @retval HAL_OK if read successful, HAL_ERROR if read failed
 */
HAL_StatusTypeDef DHT11_ReadData(uint8_t *temp, uint8_t *humi);

/**
 * @brief Initialize DHT11 GPIO pin
 */
void DHT11_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __DHT11_H */