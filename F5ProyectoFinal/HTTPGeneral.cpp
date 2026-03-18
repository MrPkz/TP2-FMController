#include "HTTPGeneral.h"
#include "HandlerHEX3653.h"
#include "HandlerTEA5767.h"
  
// ---------------- Claves de la conexión WiFi ----------------
const char* ssid = "TP2F5_AP";     // Nombre de la red que va a crear
const char* password = "12345678";   // Contraseña mínima 8 caracteres


// ---------------- Configuración de IPs ----------------
ESP8266WebServer server(80);
// Configuración IP fija para el AP
IPAddress local_IP(10, 10, 10, 10);
IPAddress gateway(10, 10, 10, 10);      // Puede ser igual a la IP local
IPAddress subnet(255, 255, 255, 0);


// ---------------- Configuración de la interfaz inicial ----------------

void handleRoot()   { server.send(200, "text/html", index_html); }


// ---------------- Levantamiento de la página ----------------

void HTTP_init() 
{
  // Crear Access Point con IP fija
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.print("Punto de acceso iniciado: ");
  Serial.println(ssid);
  Serial.print("IP del AP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);

  hexHTTP_init(); //Inicialización del HEX
  teaHTTP_init(); //Inicialización del TEA

  server.begin();
  Serial.println("Servidor HTTP iniciado");

  return;
}

String WiFiHTTP_handleReq(){
  String req;
  req = "";
  server.handleClient();
  return req;
}