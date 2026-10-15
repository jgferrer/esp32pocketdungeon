#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <Arduino.h>

enum GameState {
    STATE_TITLE,
    STATE_HERO_SELECT,
    STATE_DOOR_SELECT,
    STATE_COMBAT,
    STATE_LOOT,
    STATE_GAME_OVER,
    STATE_VICTORY
};

enum HeroClass {
    HERO_WARRIOR,
    HERO_ROGUE,
    HERO_MAGE
};

enum DieType {
    DICE_YELLOW = 0, // Fuerza (STR)
    DICE_GREEN  = 1, // Agilidad (AGI)
    DICE_BLUE   = 2, // Magia (MAG)
    DICE_HEROIC = 3  // Comodín / Heroico (WILDCARD)
};

struct Die {
    DieType type;
    uint8_t value;    // 1 a 6
    bool placed;      // Si ya está asignado a una casilla
    bool active;      // Si está disponible en la tirada actual
};

struct ChallengeBox {
    DieType req_type;       // Color requerido (o DICE_HEROIC para cualquier color)
    uint8_t min_value;      // Valor mínimo del dado
    uint8_t penalty_damage; // Daño al héroe si no se cubre
    bool filled;            // Si tiene dado colocado
    int8_t placed_die_idx;  // Índice del dado colocado (-1 si vacía)
};

#define MAX_CHALLENGE_BOXES 5
#define MAX_DICE 10

struct EncounterCard {
    const char* name;
    uint8_t floor;           // 1, 2 o 3 (Jefe)
    bool is_boss;
    uint8_t box_count;
    ChallengeBox boxes[MAX_CHALLENGE_BOXES];
    DieType loot_die_type;   // Atributo concedido si se reclama como objeto
};

struct HeroState {
    HeroClass hero_class;
    const char* name;
    int8_t hp;
    int8_t max_hp;
    uint8_t potions;
    uint8_t xp;
    uint8_t level;
    uint8_t base_yellow_dice;
    uint8_t base_green_dice;
    uint8_t base_blue_dice;
    uint8_t base_heroic_dice;
    bool skill_used;
};

#endif // GAME_TYPES_H
