
#include "definitions.h"

CONFIG_MQTT_T CONFIG_MQTT = {
    .MQTT_ADDRESS = "",
    .MQTT_USERNAME = "", 
    .MQTT_PASSWORD = "", 
    .PORT = 0,
}; 

CONFIG_WIFI_T CONFIG_WIFI = {
    .SSID = "", 
    .PASSWORD = ""
}; 

EARTHQUAKE_CONFIG_T EARTHQUAKE = {
    .EARTHQUAKE_ALARM_STATE = false, 
    .EARTHQUAKE_INDICATOR = 0, 
}; 

NETWORKSTATUS_T NETWORKSTATUS = {
    .ETHCONNECTED = false, 
    .ETHCONNECTING = false, 
    .WIFICONNECTED = false, 
    .NETWORKINTERFACECHANGED = false
}; 

SYSTEM_INFO_T SYSTEM_INFO {
    .DEVICENAME = "", 
    .DEVICEID = "", 
    .MACADDRESS= "", 
    .IP = "", 
    .USER = "", 
    .STATION = "", 
    .NETWORK = ""
}; 

ACCEL_T acceleration = {
    .x = 0, 
    .y = 0, 
    .z = 0 
}

SYSTEM_STATUS_T SYSTEM_STATUS = {
    .networkStatus = &NETWORKSTATUS, 
    .systemInfo = &SYSTEM_INFO, 
    .accelerationInfo = &acceleration 
}; 

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


bool systemInit()
{

}

