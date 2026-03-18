#include "HTTPGeneral.h"
#include "HandlerTEA5767.h"

void setup() {
  Serial.begin(115200);
  
  teafunc_init();
  
  HTTP_init();
}

String fromreq;

void loop() {
  fromreq = WiFiHTTP_handleReq();
}
