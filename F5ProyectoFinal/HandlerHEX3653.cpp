#include "HandlerHEX3653.h"

// ---------------- Función para enviar pulso ----------------

void pulso(int pin, unsigned int t) {
  digitalWrite(pin, LOW);
  delay(t);
  digitalWrite(pin, HIGH);
}

// ---------------- Handlers ----------------
void handleSUp()    { pulso(S_UP,150); server.send(200, "text/plain", "S_UP pulso enviado"); }
void handleSDown()  { pulso(S_DOWN,150); server.send(200, "text/plain", "S_DOWN pulso enviado"); }
void handleVUp()    { pulso(V_UP,150); server.send(200, "text/plain", "V_UP pulso enviado"); }
void handleVDown()  { pulso(V_DOWN,150); server.send(200, "text/plain", "V_DOWN pulso enviado"); }
void handleOnOff()  { pulso(ON_OFF,150); server.send(200, "text/plain", "ON_OFF pulso enviado"); }

// ---------------- Inicializaciones ----------------

void hexpins_init(){
    // Configurar pines
  pinMode(S_UP, OUTPUT);
  pinMode(S_DOWN, OUTPUT);
  pinMode(V_UP, OUTPUT);
  pinMode(V_DOWN, OUTPUT);
  pinMode(ON_OFF, OUTPUT);

  digitalWrite(S_UP, HIGH);
  digitalWrite(S_DOWN, HIGH);
  digitalWrite(V_UP, HIGH);
  digitalWrite(V_DOWN, HIGH);
  digitalWrite(ON_OFF, HIGH);
  
  handleOnOff(); //Llamado para que el periferico se inicialice apagado

  return;
}

void hexHTTP_init() 
{
  // Configurar rutas
  server.on("/s_up", handleSUp);
  server.on("/s_down", handleSDown);
  server.on("/v_up", handleVUp);
  server.on("/v_down", handleVDown);
  server.on("/on_off", handleOnOff);

  return;
}
