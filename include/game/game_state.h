#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <Arduino.h>
#include "config.h"
#include "game/types.h"
#include "game/dungeon_data.h"

class GameStateManager {
public:
    GameState state;
    HeroState hero;

    uint8_t floor;                    // 1, 2 o 3 (Jefe)
    uint8_t encounters_in_floor;      // Contador de encuentros en el piso actual
    uint8_t total_encounters_cleared; // Contador total para puntuación
    uint32_t score;

    EncounterCard current_encounter;
    Die dice[MAX_DICE];
    uint8_t dice_count;
    int8_t selected_die_idx;          // Dado seleccionado actualmente (-1 si ninguno)
    int8_t selected_die2_idx;         // Segundo dado seleccionado para combinación (-1 si ninguno)
    bool dice_rolled;
    bool combat_resolved;
    uint32_t end_state_time_ms;       // Debounce para fin de partida

public:
    void init();
    void select_hero(HeroClass h_class);
    void enter_next_encounter();
    void roll_dice();

    bool select_die(int8_t idx);
    bool combine_dice();
    bool place_selected_die_on_box(uint8_t box_idx);
    bool remove_die_from_box(uint8_t box_idx);
    void auto_assign_dice();

    void use_skill();
    void use_potion();
    void resolve_combat();
    void choose_loot(uint8_t option); // 0: +1 Dado, 1: Poción, 2: XP

    void handle_touch(int16_t tx, int16_t ty);
};

extern GameStateManager game;

#endif // GAME_STATE_H
