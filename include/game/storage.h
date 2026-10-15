#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>

void storage_init();
uint32_t storage_get_high_score();
void storage_save_high_score(uint32_t score);
uint16_t storage_get_victories();
void storage_increment_victories();

#endif // STORAGE_H
