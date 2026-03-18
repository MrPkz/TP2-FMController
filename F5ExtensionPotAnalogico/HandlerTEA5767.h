#include "HTTPGeneral.h"
#include "TEA5767.h"
#include <Wire.h>

// ---------------- Handlers ----------------
void updateHTML();
void handleUp();
void handleDown();
void handleMute();
void handleFreq(float f);
void handleSleep();

// ---------------- Funciones de inicialización ----------------
void teafunc_init();
void teaHTTP_init();