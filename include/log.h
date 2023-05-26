#ifndef _LOG_H_
#define _LOG_H_
#include <Arduino.h>

String _log = "";

// xSemaphoreHandle mutex_log_write;

void write_log(String data)
{
    String line;

    /*  if (xSemaphoreTake(mutex_log_write, portMAX_DELAY) == pdTRUE)
     { */

    if (_log.length() < 2000)
    {
        line = (String)millis() + ": " + data + "\n";
        _log.concat(line);
    }
    else
    {
        _log.remove(0);
    }
    /*  }
     xSemaphoreGive(mutex_log_write); */
}

String read_log()
{

    return _log;

    return "";
}

#endif //_LOG_H_