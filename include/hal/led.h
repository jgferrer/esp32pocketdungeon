#ifndef HAL_LED_H
#define HAL_LED_H

#include <Arduino.h>

void led_init();
void led_set(bool r, bool g, bool b);
void led_dungeon();
void led_combat();
void led_damage();
void led_victory();
void led_off();

#endif // HAL_LED_H
