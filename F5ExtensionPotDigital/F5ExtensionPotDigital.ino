#include "HTTPGeneral.h"
#include "HandlerTEA5767.h"
#include "HandlerX9C103S.h"

void setup() {
  Serial.begin(115200);
  
  teafunc_init();
  potfunc_init();

  HTTP_init();
}

String fromreq;

void loop() {
  fromreq = WiFiHTTP_handleReq();
}


