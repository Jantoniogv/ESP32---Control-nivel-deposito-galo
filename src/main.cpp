#include <Arduino.h>

// #include <Wire.h>

#include "config.h"
#include "wifi_functions.h"
#include "server_functions.h"
#include "lora_init.h"
#include "level_measure.h"
#include "sleep_config.h"
#include "log.h"

#include "debug_utils.h"
// #define DEBUG

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

  // Inicializa el temporizador encargado de medir el nivel del deposito periodicamente
  level_measurementTimer = xTimerCreate("level_measure", pdMS_TO_TICKS(TEME_MEASURE), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(level_measurement));
  // Inicia el timer encargado de iniciar la medida de nivel de agua
  xTimerStart(level_measurementTimer, 0);

  write_log("Temporizador de medicion del nivel iniciado...");

  // Inicializa el temporizador encargado de mandar al modo sleep al ESP32
  Sleep_Timer = xTimerCreate("Sleep_timer", pdMS_TO_TICKS(TIME_INIT_SLEEP), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(sleep_init));

  write_log("Temporizador modo sleep inicializado...");

  // Iniciamos la conexion wifi como cliente
  wifiConnectSTA();
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1));
}
