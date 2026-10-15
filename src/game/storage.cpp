#include "game/storage.h"
#include <Preferences.h>

static Preferences prefs;
static const char* PREFS_NAMESPACE = "pockdungeon";

void storage_init() {
    // Inicialización preventiva
}

uint32_t storage_get_high_score() {
    if (!prefs.begin(PREFS_NAMESPACE, true)) return 0;
    uint32_t val = prefs.getUInt("hiscore", 0);
    prefs.end();
    return val;
}

void storage_save_high_score(uint32_t score) {
    if (score <= storage_get_high_score()) return;
    if (prefs.begin(PREFS_NAMESPACE, false)) {
        prefs.putUInt("hiscore", score);
        prefs.end();
    }
}

uint16_t storage_get_victories() {
    if (!prefs.begin(PREFS_NAMESPACE, true)) return 0;
    uint16_t val = prefs.getUShort("victories", 0);
    prefs.end();
    return val;
}

void storage_increment_victories() {
    uint16_t v = storage_get_victories() + 1;
    if (prefs.begin(PREFS_NAMESPACE, false)) {
        prefs.putUShort("victories", v);
        prefs.end();
    }
}
