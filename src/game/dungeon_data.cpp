#include "game/dungeon_data.h"

// ==========================================
// PISO 1: ENCUENTROS INICIALES
// ==========================================
const EncounterCard ENCOUNTERS_FLOOR_1[TOTAL_ENCOUNTERS_F1] = {
    {
        "RATA GIGANTE", 1, false, 2,
        {
            { DICE_YELLOW, 2, 1, false, -1 },
            { DICE_GREEN,  3, 1, false, -1 }
        },
        DICE_GREEN
    },
    {
        "TRAMPA FLECHAS", 1, false, 2,
        {
            { DICE_GREEN,  4, 2, false, -1 },
            { DICE_BLUE,   2, 1, false, -1 }
        },
        DICE_GREEN
    },
    {
        "DUENDE LADRON", 1, false, 2,
        {
            { DICE_GREEN,  3, 1, false, -1 },
            { DICE_YELLOW, 3, 1, false, -1 }
        },
        DICE_YELLOW
    },
    {
        "ESQUELETO GUARDIA", 1, false, 2,
        {
            { DICE_YELLOW, 4, 1, false, -1 },
            { DICE_BLUE,   3, 1, false, -1 }
        },
        DICE_YELLOW
    },
    {
        "POZO DE PINCHOS", 1, false, 2,
        {
            { DICE_GREEN,  5, 2, false, -1 },
            { DICE_YELLOW, 2, 1, false, -1 }
        },
        DICE_BLUE
    }
};

// ==========================================
// PISO 2: ENCUENTROS AVANZADOS
// ==========================================
const EncounterCard ENCOUNTERS_FLOOR_2[TOTAL_ENCOUNTERS_F2] = {
    {
        "ORCO BERSERKER", 2, false, 3,
        {
            { DICE_YELLOW, 5, 2, false, -1 },
            { DICE_YELLOW, 4, 1, false, -1 },
            { DICE_GREEN,  3, 1, false, -1 }
        },
        DICE_YELLOW
    },
    {
        "ESPECTRO SOMBRA", 2, false, 3,
        {
            { DICE_BLUE,   5, 2, false, -1 },
            { DICE_GREEN,  4, 1, false, -1 },
            { DICE_BLUE,   3, 1, false, -1 }
        },
        DICE_BLUE
    },
    {
        "GOLEM DE ROCA", 2, false, 3,
        {
            { DICE_YELLOW, 6, 2, false, -1 },
            { DICE_YELLOW, 5, 2, false, -1 },
            { DICE_BLUE,   2, 1, false, -1 }
        },
        DICE_YELLOW
    },
    {
        "TRAMPA DE LLAMAS", 2, false, 2,
        {
            { DICE_GREEN,  5, 2, false, -1 },
            { DICE_BLUE,   4, 2, false, -1 }
        },
        DICE_BLUE
    },
    {
        "NIGROMANTE OSCURO", 2, false, 3,
        {
            { DICE_BLUE,   6, 2, false, -1 },
            { DICE_YELLOW, 4, 1, false, -1 },
            { DICE_GREEN,  4, 1, false, -1 }
        },
        DICE_BLUE
    }
};

// ==========================================
// PISO 3: JEFE FINAL
// ==========================================
const EncounterCard BOSS_ENCOUNTER = {
    "DRAGON ANCESTRAL", 3, true, 5,
    {
        { DICE_YELLOW, 6, 2, false, -1 },
        { DICE_YELLOW, 5, 2, false, -1 },
        { DICE_GREEN,  5, 2, false, -1 },
        { DICE_BLUE,   6, 2, false, -1 },
        { DICE_BLUE,   5, 2, false, -1 }
    },
    DICE_HEROIC
};

EncounterCard dungeon_get_random_card(uint8_t floor) {
    if (floor >= 3) {
        return BOSS_ENCOUNTER;
    } else if (floor == 2) {
        uint8_t idx = rand() % TOTAL_ENCOUNTERS_F2;
        return ENCOUNTERS_FLOOR_2[idx];
    } else {
        uint8_t idx = rand() % TOTAL_ENCOUNTERS_F1;
        return ENCOUNTERS_FLOOR_1[idx];
    }
}
