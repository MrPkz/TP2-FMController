#include "HandlerTEA5767.h"

TEA5767 receptor;
tea5767_info_t estado;
float freqin;

// ---------------- Handlers ----------------

void updateHTML(){
  //Agregar escritura de frecuencia, mute y standby
  String json = "{";
  json += "\"standby\": " + String(estado.standby ? "true" : "false") + ",";
  json += "\"frequency\": " + String(estado.mhz, 2) + ",";
  json += "\"mute\": " + String(estado.muted ? "true" : "false")+ ",";
  json+= "\"quality\": " + String(estado.signal_level);
  json += "}";
  server.send(200, "application/json", json);
}

void handleUp(){ 
  receptor.searchUp(); delay(80);
  receptor.getInfo(&estado);
  updateHTML();
}

void handleDown(){
  receptor.searchDown(); delay(80);
  receptor.getInfo(&estado);
  updateHTML();
}

void handleMute(){ 
  if(estado.muted) receptor.unmute();
  else receptor.mute();
  receptor.getInfo(&estado);
  updateHTML();
}

void handleFreq(float f){ 
  receptor.setFrequency(f); delay(80);
  receptor.getInfo(&estado);
  updateHTML();
}

void handleSleep(){ 
  if(estado.standby) receptor.awake();
  else receptor.standby();
  receptor.getInfo(&estado);
  updateHTML();
}

// ---------------- Inicializaciones ----------------

void teafunc_init(){
  // Inicializar el objeto receptor
  Wire.begin();
  Wire.setClock(100000);

  receptor = TEA5767();
  
  receptor.unmute();
  receptor.awake();
  receptor.setFrequency(92.1); delay(80);

  receptor.getInfo(&estado);
  return;
}

void teaHTTP_init() 
{
  // Configurar rutas
  server.on("/_up", handleUp);
  server.on("/_down", handleDown);
  server.on("/mute_on_off", handleMute);
  server.on("/set_freq", HTTP_GET, []() {
  if (!server.hasArg("v")) {
      server.send(400, "text/plain", "Falta parámetro ?v=");
      return;
    }
    float valor = server.arg("v").toFloat();
    handleFreq(valor);
  });
  server.on("/stand_by", handleSleep);
  server.on("/tea/status",updateHTML);

  return;
}
