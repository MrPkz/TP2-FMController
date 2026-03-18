#include "HandlerX9C103S.h"

X9C103S PotDigital;


// ---------------- Handlers ----------------

void updateVol(){
  //Agregar escritura de frecuencia, mute y standby
  String json = "{";
  json += "\"volume\": " + String(PotDigital.getResistance());
  json += "}";
  server.send(200, "application/json", json);
}

void handleInc1(){
  PotDigital.increaseResistance(1);
  updateVol();
}
void handleDec1(){
  PotDigital.decreaseResistance(1);
  updateVol();
}

void handleInc5(){
  PotDigital.increaseResistance(5);
  updateVol();
}
void handleDec5(){
  PotDigital.decreaseResistance(5);
  updateVol();
}

void handleInc10(){
  PotDigital.increaseResistance(10);
  updateVol();
}
void handleDec10(){
  PotDigital.decreaseResistance(10);
  updateVol();
}

void handleSet(int v){
  PotDigital.setResistance(v);
  updateVol();
}

// ---------------- Inicializaciones ----------------

void potfunc_init(){
  // Inicializar el objeto receptor

  PotDigital = X9C103S(POT_INC,POT_UD,POT_CS);
  PotDigital.initializePot();
  PotDigital.setResistance(50);
  updateVol();

  return;
}

void potHTTP_init() 
{
  // Configurar rutas

  server.on("/pot_inc1", handleInc1);
  server.on("/pot_inc5", handleInc5);
  server.on("/pot_inc10", handleInc10);

  server.on("/pot_dec1", handleDec1);
  server.on("/pot_dec5", handleDec5);
  server.on("/pot_dec10", handleDec10);

  server.on("/pot_set", HTTP_GET, []() {
  if (!server.hasArg("v")) {
      server.send(400, "text/plain", "Falta parámetro ?v=");
      return;
    }
    float valor = server.arg("v").toFloat();
    handleSet(valor);
  });

  server.on("/pot/status", HTTP_GET, updateVol);

  return;
}
