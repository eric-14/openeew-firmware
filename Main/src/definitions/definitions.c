
#include "definitions.h"

cJSON* parse_string(const char* json)
{
    cJSON* obj_json = cJSON_Parse(json); 
    if(obj_json == NULL)
    {
        ESP_LOGE(TAG, "[parse_string] Failed to parse JSON object"); 
        return 1; 
    }
}

const char* getJSONObjectItem(cJSON* json, const char* objname)
{
    cJSON* obj = cJSON_GetObjectItem(json, objname);
    return obj->valuestring; 
}


bool freeJSONObj(cJSON* json)
{
    cJsON_delete(json); 
    return true; 
}

bool RaiseEarthQuakeAlarm(EARTHqUAKER_CONFIG_T* earthquake){

  //use locks to modify the state of the alarm 
  ESP_LOGI(TAG, "Earthquake Alarm!"); 
//   strip.setBrightness(255); // The breathe intensity might have the brightness low
//   for (int i = 0; i < 10; i++)
//   {
//     if (EarthquakeAlarmBool)
//     {
//       delay(500);
//       NeoPixelStatus(AlarmLEDColor); // Alarm - blink red or orange
//       AlarmBuzzer();
//     }
//     client.loop(); // Process any incoming MQTT topics (which might stop the alarm)
//   }
//   strip.setBrightness(breatheintensity); // reset the brightness to the prior intensity
//   digitalWrite(io, LOW);     
}
