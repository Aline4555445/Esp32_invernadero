#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
//Librerias para OTA
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "../lib/ota.h"
#include "../lib/interfazweb.h"
#include "../lib/reloj.h"

//Librerias para OTA

conexionWeb *webInterface;


DateTime *reloj;
StaticJsonDocument<512> horaActual;
StaticJsonDocument<512> post_informe;
StaticJsonDocument<1024> get_informe;

const char *urlPost = "http://192.168.1.75/api-invernadero/public/api/informe"; //cambia direccion ipconfi y nombre api
const char *geturl = "http://192.168.0.21/app/public/api/resetpassword";

void setup()
{
  Serial.begin(115200);
  setupOTA("TemplateSketch");

  webInterface = new conexionWeb(ssid, passwrd);
  reloj = new DateTime();

  pinMode(2, OUTPUT);
}
int sumatoria = 0;
void loop()
{
#ifndef ESP32_RTOS
  ArduinoOTA.handle();
#endif
  sumatoria++;

  horaActual.clear();
  post_informe.clear();
  reloj->getTime();

  horaActual["hora"] = reloj->timeStringBuff;
  horaActual["Sumatoria"] = sumatoria;

//valores q se llenaran
  post_informe["id_sensor"] = random(30);
  post_informe["fecha"]= reloj->timeStringBuff; ///sera?
  post_informe["hora"]= reloj->timeStringBuff;
  post_informe["temperatura"] = random(30);
  post_informe["humedad"] = random(8);

  webInterface->webPOST(post_informe, urlPost);  //descomentarla despues de que nos genere la ip de esp32
  //  webInterface->webGET(geturl);
delay(5000);
  deserializeJson(get_informe, webInterface->POSTresponse);
  serializeJsonPretty(get_informe, Serial);
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  
  //serializeJson(horaActual,Serial);
}
