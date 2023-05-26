#ifndef _SAVE_FLASH_H_
#define _SAVE_FLASH_H_

#include <Arduino.h>

#include <Preferences.h> //https://github.com/espressif/arduino-esp32/tree/master/libraries/Preferences

Preferences preferences;

int boardId = 18;
float param = 26.5;

void saveMemFlash(String file, String key, String data)
{

    // init preference
    preferences.begin(file.c_str(), false);

    // preferences.clear(); // remove all preferences in namespace myfile
    // preferences.remove("varname");// remove varname in the namespace

    preferences.putString(key.c_str(), data.c_str());

    preferences.end();
}

String readMemFlash(String file, String key)
{
    // init preference
    preferences.begin(file.c_str(), false);

    String data = preferences.getString(key.c_str(), "");

    preferences.end();

    return data;
}

bool existKey(String file, String key)
{
    // init preference
    preferences.begin(file.c_str(), false);

    bool exist = preferences.isKey(key.c_str());

    preferences.end();

    return exist;
}

bool eraseFlash(String file)
{
    preferences.begin(file.c_str(), false);

    return preferences.clear();

    preferences.end();
}

#endif //_SAVE_FLASH_H_