#include <Arduino.h>
#include "hal/display.h"
#include "hal/buzzer.h"
#include "hal/led.h"
#include "game/game_state.h"
#include "game/storage.h"
#include "ui/renderer.h"

static bool was_touched = false;
static uint32_t last_touch_ms = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("\n[POCKET DUNGEON] Iniciando...");

    buzzer_init();
    led_init();
    display_init();
    storage_init();
    renderer_init();

    game.init();

    buzzer_click();
    Serial.println("[POCKET DUNGEON] Listo para jugar!");
}

void loop() {
    uint16_t tx = 0, ty = 0;
    bool is_touched = tft.getTouch(&tx, &ty);
    uint32_t now = millis();

    if (is_touched && !was_touched && (now - last_touch_ms >= 180)) {
        was_touched = true;
        last_touch_ms = now;
        game.handle_touch((int16_t)tx, (int16_t)ty);
    } else if (!is_touched && was_touched) {
        was_touched = false;
    }

    renderer_draw();
    delay(16);
}
