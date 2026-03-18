#include "HTTPGeneral.h"
#include "HandlerHEX3653.h"
#include "HandlerTEA5767.h"

void setup() {
  Serial.begin(115200);

  hexpins_init();
  teafunc_init();
  
  HTTP_init();
}

String fromreq;

void loop() {
  fromreq = WiFiHTTP_handleReq();
}
