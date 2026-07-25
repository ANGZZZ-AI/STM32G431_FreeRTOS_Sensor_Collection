#ifndef __LED_H
#define __LED_H

#include "main.h"

void LED_AllOff(void);
void LED_On(uint8_t index);
void LED_Off(uint8_t index);
void LED_Flash(uint8_t index);
void LED_Set(uint8_t index);
void LED_Clear(uint8_t index);
void LED_UpdateLatch(void);

#endif
