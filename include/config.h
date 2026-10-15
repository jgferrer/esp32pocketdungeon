#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ==========================================
// PANTALLA TFT 2.8" (ESP32-2432S028 CYD)
// ==========================================
#define SCREEN_WIDTH    240
#define SCREEN_HEIGHT   320
#define SCREEN_ROTATION 0 // Vertical nativo (240x320)

// Pines TFT (Bus HSPI)
#define TFT_MISO_PIN   12
#define TFT_MOSI_PIN   13
#define TFT_SCLK_PIN   14
#define TFT_CS_PIN     15
#define TFT_DC_PIN      2
#define TFT_RST_PIN    -1
#define TFT_BL_PIN     21

// Pines Panel Táctil Resistivo XPT2046 (Software SPI)
#define TOUCH_MOSI_PIN 32
#define TOUCH_MISO_PIN 39
#define TOUCH_CLK_PIN  25
#define TOUCH_CS_PIN   33
#define TOUCH_IRQ_PIN  36

// ==========================================
// PERIFÉRICOS
// ==========================================
// LED RGB Integrado (Active-LOW: 0=ON, 1=OFF)
#define LED_RED_PIN    4
#define LED_GREEN_PIN  16
#define LED_BLUE_PIN   17

// Audio Buzzer (PWM)
#define BUZZER_PIN     26
#define BUZZER_CHANNEL  0

// ==========================================
// PALETA DE COLORES RETRO DUNGEON (RGB565)
// ==========================================
#define COLOR_DUNGEON_BG     0x10A2 // Piedra oscura (#101420)
#define COLOR_CARD_BG        0x2145 // Pizarra / Pergamino oscuro (#202838)
#define COLOR_CARD_BORDER    0xC586 // Borde dorado envejecido (#C0B060)
#define COLOR_CARD_INNER     0x18E3 // Interior tarjeta

#define COLOR_DICE_YELLOW    0xFDE0 // Amarillo Fuerza (STR)
#define COLOR_DICE_GREEN     0x36E6 // Verde Agilidad (AGI)
#define COLOR_DICE_BLUE      0x34BF // Azul Magia (MAG)
#define COLOR_DICE_HEROIC    0x2965 // Gris titanio / Comodín (WILDCARD)

#define COLOR_HP_RED         0xF904 // Rojo Corazón (#F82020)
#define COLOR_POTION_CYAN    0x1E5F // Cian Mágico
#define COLOR_XP_PURPLE      0xB9DF // Púrpura Experiencia
#define COLOR_GOLD_TXT       0xFDE0 // Dorado
#define COLOR_WHITE          0xFFFF
#define COLOR_BLACK          0x0000

#endif // CONFIG_H
