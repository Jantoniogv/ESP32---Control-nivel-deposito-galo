#ifndef _SERVER_FUNCTIONS_H

#define _SERVER_FUNCTIONS_H

#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#include "save_flash.h"
#include "config.h"
#include "page_config.h"
#include "log.h"

#define DEBUG
#include "debug_utils.h"

AsyncWebServer server(80);

String htmlConfig()
{
  Config config;

  String response = (String)config_page_html;
  response.replace("%ssidAP%", config.getSsidAP());
  response.replace("%passAP%", config.getPassAP());
  response.replace("%IPap%", config.getIPap());

  response.replace("%IPap%", config.getIPap());

  if (config.getWifiType() == WIFI_MODE_AP)
  {
    response.replace(R"rawliteral(<input
            id="check_wifi"
            type="checkbox"
            name="activate_wifi"
            checked
          />)rawliteral",
                     R"rawliteral(<input
            id="check_wifi"
            type="checkbox"
            name="activate_wifi"
          />)rawliteral");
  }
  Serial.println(config.getWifiType());

  response.replace("%ssidSTA%", config.getSsidSTA());
  response.replace("%passSTA%", config.getPassSTA());

  if (config.getIPsta() != "")
  {
    response.replace("%IPsta%", config.getIPsta());
  }
  else
  {
    response.replace("%IPsta%", "");
  }

  return response;
}

void serverHandlers()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", htmlConfig()); });

  server.on("/ESPControl.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/css", config_page_css); });

  server.on("/ESPControl.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/js", config_page_js); });

  server.on("/change_conf", HTTP_POST, [](AsyncWebServerRequest *request)
            {
              Config configData;

              // List all parameters
              int params = request->params();

    for (int i = 0; i < params; i++)
    {
      AsyncWebParameter *p = request->getParam(i);
      if (p->isPost())
      {
        Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }

   // Check POST ssidAP
    if (request->hasParam("ssidAP", true))
    {
      AsyncWebParameter *p = request->getParam("ssidAP", true);
      if(p->value() != ""){
      configData.setSsidAP( p->value());
      }

      Serial.printf("POSTValue[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }

    // Check POST passAP
    if (request->hasParam("passAP", true))
    {
      AsyncWebParameter *p = request->getParam("passAP", true);
      if(p->value() != ""){
      configData.setPassAP( p->value());
      }

      Serial.printf("POSTValue[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }

 // Check POST IPap
    if (request->hasParam("IPap", true))
    {
      AsyncWebParameter *p = request->getParam("IPap", true);
      if(p->value() != ""){
      configData.setIPap( p->value());
      }

      Serial.printf("POSTValue[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }

    // Check POST activate_wifi
    if (request->hasParam("activate_wifi", true))
    {
      AsyncWebParameter *p = request->getParam("activate_wifi", true);
      if(p->value() == "on"){
      configData.setWifiType(WIFI_MODE_APSTA);
      }
      Serial.printf("POSTValue[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
      else{
        configData.setWifiType(WIFI_MODE_AP);
        Serial.println("WifiSTA=off");
      }

    // Check POST ssidSTA
    if (request->hasParam("ssidSTA", true))
    {
      AsyncWebParameter *p = request->getParam("ssidSTA", true);
      if(p->value() != ""){
      configData.setSsidSTA( p->value());
      }

      Serial.printf("POSTValue[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }
    else{
        configData.setSsidSTA("");
        Serial.println("ssidSTA=off");
      }

 // Check POST passSTA
    if (request->hasParam("passSTA", true))
    {
      AsyncWebParameter *p = request->getParam("passSTA", true);
      if(p->value() != ""){
      configData.setPassSTA( p->value());
      }

      Serial.printf("POSTValue[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }
    else{
        configData.setPassSTA("");
        Serial.println("passSTA=off");
      }

    // Check POST IPsta
    if (request->hasParam("IPsta", true))
    {
      AsyncWebParameter *p = request->getParam("IPsta", true);

      if(p->value() != ""){
      configData.setIPsta( p->value());
      }

      Serial.printf("POSTValue[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }
    else{
        configData.setIPsta("");
        Serial.println("IPsta=off");
      }

    // Guardamos los cambios
    configData.saveChange();

    //readMemFlash("config", "config");

    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", "<h2>Reiniciando...</h2>");
    request->send(response); 

//vTaskDelay(10);
    ESP.restart(); });

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              // Borra todas las claves del espacio de nombres en memoria
              eraseFlash("config");
              request->send(200, "text/html", "<h2>Restableciendo todos los valores...</h2>"); 
              
              //vTaskDelay(10);
              ESP.restart(); });

  // Endpoint que devuelve un log de los ultimos 2000 caracteres
  server.on("/log", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              String http_log=read_log();
              request->send(200, "text/plain", http_log); });

  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", favicon_ico_gz, favicon_ico_gz_len);
              response->addHeader("Content-Encoding", "gzip");
              request->send(response); });

  server.on("/logo.jpg", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              AsyncWebServerResponse *response = request->beginResponse_P(200, "image/jpeg", logo_gz, logo_gz_len);
              response->addHeader("Content-Encoding", "gzip");
              request->send(response); });
}

// Inicia el servidor web
void init_server()
{
  // Configura el servidor web
  serverHandlers();

  // Inicia ElegantOTA
  AsyncElegantOTA.begin(&server);

  // Inicia el servidor
  server.begin();

  write_log("Servidor HTTP iniciado...");

  DEBUG_PRINT("Servidor HTTP iniciado...");
}

#endif //_SERVER_FUNCTIONS_H
