#ifndef _WIFI_FUNCTIONS_H
#define _WIFI_FUNCTIONS_H
#include <Arduino.h>
#include <WiFi.h>

#include "config.h"
#include "log.h"

// #define DEBUG
#include "debug_utils.h"

void wifiConfigSTA(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = (uint32_t)0x00000000, IPAddress dns2 = (uint32_t)0x00000000)
{

    WiFi.config(local_ip, gateway, subnet, dns1, dns2);
    WiFi.reconnect();
}

bool wifiConfigAP(IPAddress ip, IPAddress subnet = IPAddress(255, 255, 255, 0))
{
    bool status = WiFi.softAPConfig(ip, ip, subnet);

    return status;
}

void wifiConnectAP()
{
    Config configData;

    WiFi.softAP(configData.getSsidAP(), configData.getPassAP());

    // Asignamos la IP del punto de acceso
    IPAddress IPap;
    IPap.fromString(configData.getIPap());

    wifiConfigAP(IPap);

    write_log("IP as soft AP: " + WiFi.softAPIP().toString());

    DEBUG_PRINT("SsidAP: " + static_cast<String>(configData.getSsidAP()));

    DEBUG_PRINT("PassAP: " + static_cast<String>(configData.getPassAP()));

    DEBUG_PRINT("IP as soft AP: " + WiFi.softAPIP().toString());
}

bool wifiConnectSTA() // wifi_mode_t mode, const char *ssidSTA, const char *passSTA
{
    Config configData;

    if (configData.getWifiType() != WIFI_MODE_AP)
    {
        WiFi.begin(configData.getSsidSTA(), configData.getPassSTA());

        if (WiFi.status() != WL_CONNECTED)
        {

            DEBUG_PRINT("Connecting");

            int n = 0;
            while (WiFi.status() != WL_CONNECTED && n < 20)
            {
                vTaskDelay(pdMS_TO_TICKS(1000));
                DEBUG_PRINT(".");
                n++;
            }
        }

        if (WiFi.status() == WL_CONNECTED)
        {

            // Configurar la IP del modo cliente en caso de configurar una IP fija
            if (configData.getIPsta() != initIPsta)
            {
                IPAddress IP;
                IP.fromString(configData.getIPap());

                IPAddress gateway;
                gateway.fromString(configData.getGateway());

                IPAddress subnet;
                subnet.fromString(configData.getSubnet());

                wifiConfigSTA(IP, gateway, subnet);
            }

            return true;
        }
    }
    return false;
}

void WiFiEvent(WiFiEvent_t event)
{
    DEBUG_PRINT("[WiFi-event] event: ");
    DEBUG_PRINT(event);

    switch (event)
    {
    case SYSTEM_EVENT_STA_GOT_IP:

        DEBUG_PRINT("WiFi connected");
        DEBUG_PRINT("IP as soft STA: ");
        DEBUG_PRINT(WiFi.localIP());

        write_log("IP as soft STA: ");
        write_log(WiFi.localIP().toString());
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:

        DEBUG_PRINT("WiFi lost connection");
        write_log("WiFi lost connection");

        WiFi.reconnect();
        break;
    }
}

#endif //_WIFI_FUNCTIONS_H