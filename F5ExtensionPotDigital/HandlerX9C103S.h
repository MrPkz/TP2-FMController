#include "HTTPGeneral.h"
#include "X9C103S.h"
#include <Wire.h>

#define POT_CS 13 // D7
#define POT_UD 12   // D6
#define POT_INC 14 // D5

// ---------------- Handlers ----------------
void updateVol();
void handleInc1();
void handleInc5();
void handleInc10();
void handleDec1();
void handleDec5();
void handleDec10();
void handleSet(int val);

// ---------------- Funciones de inicialización ----------------
void potfunc_init();
void potHTTP_init();