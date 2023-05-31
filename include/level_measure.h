#ifndef _LEVEL_MEASURE_H_
#define _LEVEL_MEASURE_H_

#include "Arduino.h"

#include "lora_send.h"

#include "debug_utils.h"
#include "log.h"

#define DEBUG

// Topic enviar nivel
const char nivelDepGaloBajo[] = "nivel/deposito/galo/bajo";

TimerHandle_t level_measurementTimer;

//**********************************************************//
//***** NIVEL POR ULTRASONIDOS *****************************//
//**********************************************************//

// Configuramos los pines del sensor Trigger y Echo
#define PIN_TX 25
#define PIN_RX 36

#define Serial_US Serial1

// Número de muestras
// #define N_SAMPLES 5

// Distancia a los lleno y vacío
#define FULL_DISTANCE 250
#define EMPTY_DISTANCE 3150

// Método que inicia la secuencia del Trigger para comenzar a medir
void init_ultrasonic_sensor()
{
    Serial_US.begin(9600, SERIAL_8N1, PIN_RX, PIN_TX);

    DEBUG_PRINT("Level measure init...");

    write_log("Level measure init");
}

// Funcion que obtiene la distancia libre de agua que queda en el deposito
// Toma 5 muestras y obtiene la media de las medidas
void level_measurement()
{
    // Variable que almacena la distancia media
    int distance = 0;

    // Variable que almacena los datos enviados desde el medidor de ultrasonidos
    int buffer[4] = {0};

    bool medida_correcta = false;

    while (!medida_correcta)
    {
        // Vacia el buffer
        while (Serial_US.available())
        {
            Serial_US.read();
        }

        // Espera 500 ms
        vTaskDelay(pdMS_TO_TICKS(500));

        int i = 0;
        while (Serial_US.available() && i < 4)
        {
            buffer[i] = Serial_US.read();

            i++;
        }

        Serial.println("");

        for (int k = 0; k < 4; k++)
        {
            Serial.print(buffer[k]);
            Serial.print("   ");
        }

        // Obtiene la distancia medida
        if (buffer[0] == 255)
        {
            DEBUG_PRINT("Medida distancia correcta");

            distance += buffer[1] * 256 + buffer[2];
            medida_correcta = true;
        }
        else
        {
            DEBUG_PRINT("Medida distancia incorrecta");
        }
    }

    // Calcula el nivel de liquido y lo envia mediante LoRa
    int measure_water = map((EMPTY_DISTANCE - distance), 0, (EMPTY_DISTANCE - FULL_DISTANCE), 0, 100);

    if (measure_water < 0)
    {
        measure_water = 0;
    }
    if (measure_water > 100)
    {
        measure_water = 100;
    }

    // Envia los datos mediante lora
    sendDataLora((String)nivelDepGaloBajo + "=" + (String)measure_water);

    DEBUG_PRINT(distance);
    DEBUG_PRINT(" cm");

    DEBUG_PRINT(measure_water);
    DEBUG_PRINT(" %");
}

#endif // _LEVEL_MEASURE_H_
