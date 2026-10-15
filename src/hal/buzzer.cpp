#include "hal/buzzer.h"
#include "config.h"

static bool sound_enabled = true;

void buzzer_init() {
    ledcSetup(BUZZER_CHANNEL, 2000, 8);
    ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
    ledcWrite(BUZZER_CHANNEL, 0);
}

void buzzer_set_enabled(bool enabled) {
    sound_enabled = enabled;
    if (!sound_enabled) {
        ledcWrite(BUZZER_CHANNEL, 0);
    }
}

bool buzzer_is_enabled() {
    return sound_enabled;
}

static void play_tone(uint32_t freq, uint32_t ms) {
    if (!sound_enabled || freq == 0) return;
    ledcWriteTone(BUZZER_CHANNEL, freq);
    delay(ms);
    ledcWrite(BUZZER_CHANNEL, 0);
}

void buzzer_click() {
    play_tone(2400, 6);
}

void buzzer_dice_roll() {
    if (!sound_enabled) return;
    for (int i = 0; i < 4; i++) {
        ledcWriteTone(BUZZER_CHANNEL, 1200 + (rand() % 800));
        delay(18);
        ledcWrite(BUZZER_CHANNEL, 0);
        delay(12);
    }
}

void buzzer_dice_place() {
    if (!sound_enabled) return;
    ledcWriteTone(BUZZER_CHANNEL, 1400);
    delay(15);
    ledcWriteTone(BUZZER_CHANNEL, 1900);
    delay(20);
    ledcWrite(BUZZER_CHANNEL, 0);
}

void buzzer_damage() {
    if (!sound_enabled) return;
    ledcWriteTone(BUZZER_CHANNEL, 220);
    delay(60);
    ledcWriteTone(BUZZER_CHANNEL, 140);
    delay(90);
    ledcWrite(BUZZER_CHANNEL, 0);
}

void buzzer_skill() {
    if (!sound_enabled) return;
    ledcWriteTone(BUZZER_CHANNEL, 1000);
    delay(20);
    ledcWriteTone(BUZZER_CHANNEL, 1500);
    delay(25);
    ledcWriteTone(BUZZER_CHANNEL, 2100);
    delay(35);
    ledcWrite(BUZZER_CHANNEL, 0);
}

void buzzer_potion() {
    if (!sound_enabled) return;
    ledcWriteTone(BUZZER_CHANNEL, 1200);
    delay(25);
    ledcWriteTone(BUZZER_CHANNEL, 1600);
    delay(30);
    ledcWriteTone(BUZZER_CHANNEL, 2000);
    delay(40);
    ledcWrite(BUZZER_CHANNEL, 0);
}

void buzzer_victory() {
    if (!sound_enabled) return;
    const uint16_t notes[] = { 523, 659, 784, 1046, 1318, 1568 };
    const uint8_t dur[] = { 60,  60,  60,  80,   80,   200 };
    for (size_t i = 0; i < sizeof(notes)/sizeof(notes[0]); i++) {
        ledcWriteTone(BUZZER_CHANNEL, notes[i]);
        delay(dur[i]);
        ledcWrite(BUZZER_CHANNEL, 0);
        delay(15);
    }
}

void buzzer_game_over() {
    if (!sound_enabled) return;
    const uint16_t notes[] = { 440, 392, 349, 293 };
    const uint8_t dur[] = { 100, 100, 100, 250 };
    for (size_t i = 0; i < sizeof(notes)/sizeof(notes[0]); i++) {
        ledcWriteTone(BUZZER_CHANNEL, notes[i]);
        delay(dur[i]);
        ledcWrite(BUZZER_CHANNEL, 0);
        delay(20);
    }
}
