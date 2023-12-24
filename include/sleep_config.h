#ifndef _SLEEP_CONFIG_H_
#define _SLEEP_CONFIG_H_

#include "Arduino.h"

// Variables configuracion modo sleep
#define uS_a_S_FACTOR 1000000 /* 1000000 microsegundos / segundo */

#define S_a_M_FACTOR 60 /* 60 segundos / minuto para tener segundos */

#define TIME_SLEEP 2 /* Tiempo que duerme en minutos */

#define TIME_INIT_SLEEP 60000 // Tiempo de demora para iniciar el modo sleep desde inicio del timer

TimerHandle_t Sleep_Timer;

// Funcion que manda al ESP32 al modo sleep
void sleep_init()
{
    //***** Configuramos el modo sleep del ESP32 *****//
    esp_sleep_enable_timer_wakeup(TIME_SLEEP * S_a_M_FACTOR * uS_a_S_FACTOR);

    // ESP32 a dormir
    esp_deep_sleep_start();
}

#endif // _SLEEP_CONFIG_H_