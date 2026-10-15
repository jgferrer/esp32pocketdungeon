#include "game/game_state.h"
#include "game/storage.h"
#include "hal/buzzer.h"
#include "hal/led.h"
#include "ui/renderer.h"

GameStateManager game;

void GameStateManager::init() {
    state = STATE_TITLE;
    floor = 1;
    encounters_in_floor = 0;
    total_encounters_cleared = 0;
    score = 0;
    selected_die_idx = -1;
    selected_die2_idx = -1;
    dice_rolled = false;
    combat_resolved = false;
    dice_count = 0;
    renderer_reset();
    led_dungeon();
}

void GameStateManager::select_hero(HeroClass h_class) {
    hero.hero_class = h_class;
    hero.level = 1;
    hero.xp = 0;
    hero.potions = 1;
    hero.skill_used = false;

    if (h_class == HERO_WARRIOR) {
        hero.name = "GUERRERO";
        hero.max_hp = 12;
        hero.base_yellow_dice = 3;
        hero.base_green_dice  = 1;
        hero.base_blue_dice   = 1;
        hero.base_heroic_dice = 0;
    } else if (h_class == HERO_ROGUE) {
        hero.name = "PICARA";
        hero.max_hp = 10;
        hero.base_yellow_dice = 1;
        hero.base_green_dice  = 3;
        hero.base_blue_dice   = 1;
        hero.base_heroic_dice = 0;
    } else { // HERO_MAGE
        hero.name = "MAGA";
        hero.max_hp = 8;
        hero.base_yellow_dice = 1;
        hero.base_green_dice  = 1;
        hero.base_blue_dice   = 3;
        hero.base_heroic_dice = 0;
    }
    hero.hp = hero.max_hp;

    state = STATE_DOOR_SELECT;
    renderer_reset();
    buzzer_skill();
}

void GameStateManager::enter_next_encounter() {
    encounters_in_floor++;
    if (floor < 3 && encounters_in_floor > 3) {
        floor++;
        encounters_in_floor = 1;
    }

    current_encounter = dungeon_get_random_card(floor);
    for (uint8_t i = 0; i < current_encounter.box_count; i++) {
        current_encounter.boxes[i].filled = false;
        current_encounter.boxes[i].placed_die_idx = -1;
    }

    dice_rolled = false;
    combat_resolved = false;
    selected_die_idx = -1;
    selected_die2_idx = -1;
    hero.skill_used = false;

    state = STATE_COMBAT;
    renderer_reset();
    led_combat();
    buzzer_click();
}

void GameStateManager::roll_dice() {
    if (dice_rolled) return;

    dice_count = 0;
    // Agregar dados de Fuerza
    for (uint8_t i = 0; i < hero.base_yellow_dice && dice_count < MAX_DICE; i++) {
        dice[dice_count++] = { DICE_YELLOW, (uint8_t)(1 + (rand() % 6)), false, true };
    }
    // Agregar dados de Agilidad
    for (uint8_t i = 0; i < hero.base_green_dice && dice_count < MAX_DICE; i++) {
        dice[dice_count++] = { DICE_GREEN, (uint8_t)(1 + (rand() % 6)), false, true };
    }
    // Agregar dados de Magia
    for (uint8_t i = 0; i < hero.base_blue_dice && dice_count < MAX_DICE; i++) {
        dice[dice_count++] = { DICE_BLUE, (uint8_t)(1 + (rand() % 6)), false, true };
    }
    // Agregar dados Heroicos
    for (uint8_t i = 0; i < hero.base_heroic_dice && dice_count < MAX_DICE; i++) {
        dice[dice_count++] = { DICE_HEROIC, (uint8_t)(1 + (rand() % 6)), false, true };
    }

    dice_rolled = true;
    selected_die_idx = -1;
    selected_die2_idx = -1;
    buzzer_dice_roll();
}

bool GameStateManager::select_die(int8_t idx) {
    if (idx < 0 || idx >= dice_count) {
        selected_die_idx = -1;
        selected_die2_idx = -1;
        return false;
    }
    if (dice[idx].placed || !dice[idx].active) {
        return false;
    }

    // Si no había ningún dado seleccionado:
    if (selected_die_idx < 0) {
        selected_die_idx = idx;
        selected_die2_idx = -1;
        buzzer_click();
        return true;
    }

    // Si se vuelve a tocar el primer dado seleccionado:
    if (selected_die_idx == idx) {
        if (selected_die2_idx >= 0) {
            selected_die_idx = selected_die2_idx;
            selected_die2_idx = -1;
        } else {
            selected_die_idx = -1;
        }
        buzzer_click();
        return true;
    }

    // Si se vuelve a tocar el segundo dado seleccionado:
    if (selected_die2_idx == idx) {
        selected_die2_idx = -1;
        buzzer_click();
        return true;
    }

    // Se toca otro dado: si coincide en color y no es heroico, seleccionamos ambos para combinar
    if (dice[idx].type == dice[selected_die_idx].type && dice[idx].type != DICE_HEROIC) {
        selected_die2_idx = idx;
        buzzer_click();
        return true;
    }

    // Color distinto o dado heroico: cambiamos la selección al nuevo dado
    selected_die_idx = idx;
    selected_die2_idx = -1;
    buzzer_click();
    return true;
}

bool GameStateManager::combine_dice() {
    if (selected_die_idx < 0 || selected_die2_idx < 0) return false;
    if (selected_die_idx >= dice_count || selected_die2_idx >= dice_count) return false;
    if (selected_die_idx == selected_die2_idx) return false;

    Die& d1 = dice[selected_die_idx];
    Die& d2 = dice[selected_die2_idx];

    if (d1.placed || !d1.active || d2.placed || !d2.active) return false;
    if (d1.type != d2.type || d1.type == DICE_HEROIC) return false;

    uint8_t idx1 = (selected_die_idx < selected_die2_idx) ? selected_die_idx : selected_die2_idx;
    uint8_t idx2 = (selected_die_idx < selected_die2_idx) ? selected_die2_idx : selected_die_idx;

    uint8_t min_val = (dice[idx1].value < dice[idx2].value) ? dice[idx1].value : dice[idx2].value;

    // Convertir el primer dado en heroico con el menor valor
    dice[idx1].type = DICE_HEROIC;
    dice[idx1].value = min_val;
    dice[idx1].placed = false;
    dice[idx1].active = true;

    // Eliminar el segundo dado desplazando los restantes
    for (uint8_t i = idx2; i < dice_count - 1; i++) {
        dice[i] = dice[i + 1];
    }
    dice_count--;

    // Actualizar índices de dados colocados en las casillas
    for (uint8_t b = 0; b < current_encounter.box_count; b++) {
        if (current_encounter.boxes[b].filled && current_encounter.boxes[b].placed_die_idx > idx2) {
            current_encounter.boxes[b].placed_die_idx--;
        }
    }

    selected_die_idx = idx1;
    selected_die2_idx = -1;

    buzzer_skill();
    return true;
}

bool GameStateManager::place_selected_die_on_box(uint8_t box_idx) {
    if (selected_die_idx < 0 || selected_die_idx >= dice_count) return false;
    if (box_idx >= current_encounter.box_count) return false;
    if (current_encounter.boxes[box_idx].filled) return false;

    Die& d = dice[selected_die_idx];
    ChallengeBox& b = current_encounter.boxes[box_idx];

    // Verificar si el dado cumple color y valor
    bool color_ok = (d.type == DICE_HEROIC || b.req_type == DICE_HEROIC || d.type == b.req_type);
    bool value_ok = (d.value >= b.min_value);

    if (color_ok && value_ok) {
        b.filled = true;
        b.placed_die_idx = selected_die_idx;
        d.placed = true;
        if (selected_die2_idx >= 0) {
            selected_die_idx = selected_die2_idx;
            selected_die2_idx = -1;
        } else {
            selected_die_idx = -1;
        }
        buzzer_dice_place();
        return true;
    } else if (selected_die2_idx >= 0 && selected_die2_idx < dice_count) {
        // Si el primero no cupo pero hay un segundo seleccionado, probar con el segundo
        Die& d2 = dice[selected_die2_idx];
        bool color2_ok = (d2.type == DICE_HEROIC || b.req_type == DICE_HEROIC || d2.type == b.req_type);
        bool value2_ok = (d2.value >= b.min_value);
        if (color2_ok && value2_ok) {
            b.filled = true;
            b.placed_die_idx = selected_die2_idx;
            d2.placed = true;
            selected_die2_idx = -1;
            buzzer_dice_place();
            return true;
        }
    }

    buzzer_damage();
    return false;
}

bool GameStateManager::remove_die_from_box(uint8_t box_idx) {
    if (box_idx >= current_encounter.box_count) return false;
    ChallengeBox& b = current_encounter.boxes[box_idx];
    if (!b.filled || b.placed_die_idx < 0) return false;

    dice[b.placed_die_idx].placed = false;
    b.filled = false;
    b.placed_die_idx = -1;
    buzzer_click();
    return true;
}

void GameStateManager::auto_assign_dice() {
    if (!dice_rolled) return;

    for (uint8_t b = 0; b < current_encounter.box_count; b++) {
        if (current_encounter.boxes[b].filled) continue;

        int8_t best_die = -1;
        uint8_t min_surplus = 255;

        for (uint8_t d = 0; d < dice_count; d++) {
            if (dice[d].placed || !dice[d].active) continue;

            bool color_ok = (dice[d].type == DICE_HEROIC || current_encounter.boxes[b].req_type == DICE_HEROIC || dice[d].type == current_encounter.boxes[b].req_type);
            bool value_ok = (dice[d].value >= current_encounter.boxes[b].min_value);

            if (color_ok && value_ok) {
                uint8_t surplus = dice[d].value - current_encounter.boxes[b].min_value;
                if (surplus < min_surplus) {
                    min_surplus = surplus;
                    best_die = d;
                }
            }
        }

        if (best_die >= 0) {
            current_encounter.boxes[b].filled = true;
            current_encounter.boxes[b].placed_die_idx = best_die;
            dice[best_die].placed = true;
        }
    }
    selected_die_idx = -1;
    selected_die2_idx = -1;
    buzzer_dice_place();
}

void GameStateManager::use_skill() {
    if (hero.skill_used || !dice_rolled) return;

    if (hero.hero_class == HERO_WARRIOR) {
        // Golpe de Escudo: aumenta +2 el valor del dado más bajo de fuerza (máx 6)
        int8_t target = -1;
        uint8_t min_v = 7;
        for (uint8_t i = 0; i < dice_count; i++) {
            if (!dice[i].placed && dice[i].type == DICE_YELLOW && dice[i].value < min_v) {
                min_v = dice[i].value;
                target = i;
            }
        }
        if (target >= 0) {
            dice[target].value = (dice[target].value + 2 > 6) ? 6 : (dice[target].value + 2);
            hero.skill_used = true;
            selected_die_idx = -1;
            selected_die2_idx = -1;
            buzzer_skill();
        }
    } else if (hero.hero_class == HERO_ROGUE) {
        // Paso Sombrío: convierte un dado de agilidad en dado comodín (HEROIC)
        for (uint8_t i = 0; i < dice_count; i++) {
            if (!dice[i].placed && dice[i].type == DICE_GREEN) {
                dice[i].type = DICE_HEROIC;
                hero.skill_used = true;
                selected_die_idx = -1;
                selected_die2_idx = -1;
                buzzer_skill();
                break;
            }
        }
    } else { // HERO_MAGE
        // Transmutación: convierte el dado de menor valor en comodín de valor 6
        int8_t target = -1;
        uint8_t min_v = 7;
        for (uint8_t i = 0; i < dice_count; i++) {
            if (!dice[i].placed && dice[i].value < min_v) {
                min_v = dice[i].value;
                target = i;
            }
        }
        if (target >= 0) {
            dice[target].type = DICE_HEROIC;
            dice[target].value = 6;
            hero.skill_used = true;
            selected_die_idx = -1;
            selected_die2_idx = -1;
            buzzer_skill();
        }
    }
}

void GameStateManager::use_potion() {
    if (hero.potions == 0 || hero.hp >= hero.max_hp) return;
    hero.potions--;
    hero.hp += 4;
    if (hero.hp > hero.max_hp) hero.hp = hero.max_hp;
    buzzer_potion();
}

void GameStateManager::resolve_combat() {
    if (!dice_rolled) return;

    int8_t total_damage = 0;
    for (uint8_t i = 0; i < current_encounter.box_count; i++) {
        if (!current_encounter.boxes[i].filled) {
            total_damage += current_encounter.boxes[i].penalty_damage;
        }
    }

    if (total_damage > 0) {
        hero.hp -= total_damage;
        buzzer_damage();
        led_damage();
    } else {
        buzzer_victory();
        led_victory();
    }

    if (hero.hp <= 0) {
        hero.hp = 0;
        state = STATE_GAME_OVER;
        end_state_time_ms = millis();
        storage_save_high_score(score);
        buzzer_game_over();
        return;
    }

    // Encuentro superado con éxito
    total_encounters_cleared++;
    score += (floor * 100) + (hero.hp * 10);

    if (current_encounter.is_boss) {
        state = STATE_VICTORY;
        end_state_time_ms = millis();
        score += 1000;
        storage_save_high_score(score);
        storage_increment_victories();
        buzzer_victory();
        return;
    }

    state = STATE_LOOT;
    renderer_reset();
}

void GameStateManager::choose_loot(uint8_t option) {
    if (option == 0) {
        // Objeto: +1 dado del tipo del monstruo
        if (current_encounter.loot_die_type == DICE_YELLOW) hero.base_yellow_dice++;
        else if (current_encounter.loot_die_type == DICE_GREEN) hero.base_green_dice++;
        else if (current_encounter.loot_die_type == DICE_BLUE) hero.base_blue_dice++;
        else hero.base_heroic_dice++;
        buzzer_skill();
    } else if (option == 1) {
        // Poción / Curación
        hero.hp += 3;
        if (hero.hp > hero.max_hp) hero.hp = hero.max_hp;
        if (hero.potions < 2) hero.potions++;
        buzzer_potion();
    } else {
        // Experiencia (XP)
        hero.xp++;
        if (hero.xp % 2 == 0) {
            hero.level++;
            hero.max_hp += 2;
            hero.hp += 2;
            hero.base_heroic_dice++;
        }
        buzzer_dice_place();
    }

    state = STATE_DOOR_SELECT;
    renderer_reset();
    led_dungeon();
}

void GameStateManager::handle_touch(int16_t tx, int16_t ty) {
    // -------------------------------------------------------------
    // PANTALLA DE TÍTULO
    // -------------------------------------------------------------
    if (state == STATE_TITLE) {
        state = STATE_HERO_SELECT;
        buzzer_click();
        renderer_reset();
        return;
    }

    // -------------------------------------------------------------
    // SELECCIÓN DE HÉROE
    // -------------------------------------------------------------
    if (state == STATE_HERO_SELECT) {
        if (ty >= 55 && ty <= 125) {
            select_hero(HERO_WARRIOR);
        } else if (ty >= 135 && ty <= 205) {
            select_hero(HERO_ROGUE);
        } else if (ty >= 215 && ty <= 285) {
            select_hero(HERO_MAGE);
        }
        return;
    }

    // -------------------------------------------------------------
    // SELECCIÓN DE PUERTA / EXPLORACIÓN
    // -------------------------------------------------------------
    if (state == STATE_DOOR_SELECT) {
        // Puerta Izquierda: X = 15..110, Y = 75..220
        if (tx >= 15 && tx <= 110 && ty >= 75 && ty <= 220) {
            enter_next_encounter();
            return;
        }
        // Puerta Derecha: X = 130..225, Y = 75..220
        if (tx >= 130 && tx <= 225 && ty >= 75 && ty <= 220) {
            enter_next_encounter();
            return;
        }
        // Botón Poción en HUD o bandeja
        if (ty >= 240 && ty <= 280 && tx >= 60 && tx <= 180) {
            use_potion();
            return;
        }
        return;
    }

    // -------------------------------------------------------------
    // COMBATE
    // -------------------------------------------------------------
    if (state == STATE_COMBAT) {
        // 0. Botón FUSIONAR en la cabecera de la bandeja
        if (dice_rolled && selected_die_idx >= 0 && selected_die2_idx >= 0) {
            if (tx >= 115 && tx <= 235 && ty >= 175 && ty <= 198) {
                combine_dice();
                return;
            }
        }

        // 1. Tocar casillas de la carta (Y = 100 a 170)
        if (ty >= 95 && ty <= 170) {
            uint8_t count = current_encounter.box_count;
            int16_t box_w = 40;
            int16_t start_x = 120 - (count * (box_w + 6)) / 2;

            for (uint8_t b = 0; b < count; b++) {
                int16_t bx = start_x + b * (box_w + 6);
                if (tx >= bx && tx <= bx + box_w) {
                    if (current_encounter.boxes[b].filled) {
                        remove_die_from_box(b);
                    } else if (selected_die_idx >= 0) {
                        place_selected_die_on_box(b);
                    }
                    return;
                }
            }
        }

        // 2. Tocar dados en la bandeja (Y = 197 a 265)
        if (dice_rolled && ty >= 197 && ty <= 265) {
            int16_t die_w = 32;
            int16_t start_x = 12;
            for (uint8_t d = 0; d < dice_count; d++) {
                int16_t dx = start_x + (d % 6) * 37;
                int16_t dy = 198 + (d / 6) * 32;
                if (tx >= dx && tx <= dx + die_w && ty >= dy && ty <= dy + die_w) {
                    select_die(d);
                    return;
                }
            }
        }

        // 3. Botones de Acción (Y = 265 a 310)
        if (ty >= 260 && ty <= 310) {
            if (!dice_rolled) {
                // Botón grande: TIRAR DADOS
                if (tx >= 20 && tx <= 220) {
                    roll_dice();
                }
            } else {
                // Botón 1: AUTO-ASIGNAR (X = 8..74)
                if (tx >= 8 && tx <= 74) {
                    auto_assign_dice();
                    return;
                }
                // Botón 2: HABILIDAD (X = 80..152)
                if (tx >= 80 && tx <= 152) {
                    use_skill();
                    return;
                }
                // Botón 3: RESOLVER (X = 158..232)
                if (tx >= 158 && tx <= 232) {
                    resolve_combat();
                    return;
                }
            }
        }
        return;
    }

    // -------------------------------------------------------------
    // FASE DE BOTÍN
    // -------------------------------------------------------------
    if (state == STATE_LOOT) {
        if (ty >= 80 && ty <= 135) {
            choose_loot(0); // +1 Dado
        } else if (ty >= 145 && ty <= 200) {
            choose_loot(1); // Poción
        } else if (ty >= 210 && ty <= 265) {
            choose_loot(2); // XP
        }
        return;
    }

    // -------------------------------------------------------------
    // FIN DE JUEGO / VICTORIA
    // -------------------------------------------------------------
    if (state == STATE_GAME_OVER || state == STATE_VICTORY) {
        if (millis() - end_state_time_ms < 800) return;
        if (tx >= 30 && tx <= 210 && ty >= 165 && ty <= 225) {
            init();
        }
        return;
    }
}
