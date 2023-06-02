#include <Arduino.h>

// #include <Wire.h>

#include "config.h"
#include "wifi_functions.h"
#include "server_functions.h"
#include "lora_init.h"
#include "level_measure.h"
#include "log.h"

#include "debug_utils.h"
#define DEBUG

void setup()
{
  // Borra todas las claves del espacio de nombres en memoria
  // eraseFlash("config");

  // Inicializa el objeto con la variables de configuracion
  Config configData;

  // Inicia la conexion serial
  Serial.begin(115200);

  // Inicia el modulo LoRa
  init_lora();

  // Se captura los eventos de la conexion wifi
  WiFi.onEvent(WiFiEvent);

  // Configuramos el modo del wifi
  WiFi.mode(configData.getWifiType());

  // Iniciamos la conexion wifi como punto de acceso
  wifiConnectAP();

  // Inicia y configura el servidor web
  init_server();

  // Inicia el sensor de ultrasonidos encargado de medir el nivel de agua en el deposito
  init_ultrasonic_sensor();

  // Inicia el temporizador encargado de medir el nivel del deposito periodicamente
  level_measurementTimer = xTimerCreate("level_measure", pdMS_TO_TICKS(10000), pdTRUE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(level_measurement));
  xTimerStart(level_measurementTimer, 0);

  write_log("Temporizador de medicion del nivel iniciado...");

  // Iniciamos la conexion wifi como cliente
  wifiConnectSTA();
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1));
}
