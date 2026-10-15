#include "hal/led.h"
#include "config.h"

void led_init() {
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_BLUE_PIN, OUTPUT);
    led_off();
}

void led_set(bool r, bool g, bool b) {
    // Active-LOW
    digitalWrite(LED_RED_PIN, r ? LOW : HIGH);
    digitalWrite(LED_GREEN_PIN, g ? LOW : HIGH);
    digitalWrite(LED_BLUE_PIN, b ? LOW : HIGH);
}

void led_dungeon() {
    led_set(false, false, true); // Azul tenue misterioso
}

void led_combat() {
    led_set(true, true, false); // Amarillo alerta
}

void led_damage() {
    led_set(true, false, false); // Rojo peligro
}

void led_victory() {
    led_set(false, true, false); // Verde triunfo
}

void led_off() {
    led_set(false, false, false);
}
