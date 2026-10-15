#ifndef DUNGEON_DATA_H
#define DUNGEON_DATA_H

#include "game/types.h"

#define TOTAL_ENCOUNTERS_F1 5
#define TOTAL_ENCOUNTERS_F2 5
#define TOTAL_BOSSES        1

extern const EncounterCard ENCOUNTERS_FLOOR_1[TOTAL_ENCOUNTERS_F1];
extern const EncounterCard ENCOUNTERS_FLOOR_2[TOTAL_ENCOUNTERS_F2];
extern const EncounterCard BOSS_ENCOUNTER;

EncounterCard dungeon_get_random_card(uint8_t floor);

#endif // DUNGEON_DATA_H
