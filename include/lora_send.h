#ifndef _LORA_SEND_H_
#define _LORA_SEND_H_
#include <Arduino.h>

#include "lora_init.h"
#include "log.h"

#include "debug_utils.h"
#define DEBUG

void sendDataLora(String data)
{
    DEBUG_PRINT(data);

    // send packet
    LoRa.beginPacket();
    LoRa.print(data);
    LoRa.endPacket();

    write_log("send LoRa: " + data);
}

#endif //_LORA_SEND_H_