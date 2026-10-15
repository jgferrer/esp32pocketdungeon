#ifndef HAL_BUZZER_H
#define HAL_BUZZER_H

#include <Arduino.h>

void buzzer_init();
void buzzer_set_enabled(bool enabled);
bool buzzer_is_enabled();

void buzzer_click();
void buzzer_dice_roll();
void buzzer_dice_place();
void buzzer_damage();
void buzzer_skill();
void buzzer_potion();
void buzzer_victory();
void buzzer_game_over();

#endif // HAL_BUZZER_H
