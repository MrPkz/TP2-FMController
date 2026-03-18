#include "HTTPGeneral.h"

// ---------------- Pines del HEX ----------------
#define S_UP 15   // D8
#define S_DOWN 13 // D7
#define V_UP 12   // D6
#define V_DOWN 14 // D5
#define ON_OFF 16 // D0


// ---------------- Envío de pulso a pin indicado ----------------
void pulso(int, unsigned int);

// ---------------- Handlers ----------------
void handleRoot();
void handleSUp();
void handleSDown();
void handleVUp();
void handleVDown();
void handleOnOff();

// ---------------- Funciones de inicialización ----------------
void hexpins_init();
void hexHTTP_init();