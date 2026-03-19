
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
    .accelerationInfo = &acceleration, 
    .TAG = "MAIN", 
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

bool raiseEarthQuakeAlarm(EARTHqUAKER_CONFIG_T* earthquake){

  //use locks to modify the state of the alarm 
  ESP_LOGI("RaiseEarthQuakeAlarm", "Earthquake Alarm!"); 
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




QueueHandle_t sensorDataQueue = NULL; 
QueueHandle_t systemInfoQueue = NULL; 
QueueHandle_t alarmInfoQueue = NULL;  

bool systemQueueInit()
{
    sensorDataQueue = xQueueCreate(1, sizeof(ACCEL_T)); 
    systemStatusQueue = xQueueCreate(1, sizeof(SYSTEM_STATUS_T)); 
    alarmInfoQueue = xQueueCreate(1, sizeof(EARTHQUAKE_CONFIG_T)); 
    if (sensorDataQueue == NULL || systemStatusQueue == NULL || alarmInfoQueue== NULL)
    {
        //error in creation of queues 
        ESP_LOGE(SYSTEM_STATUS.TAG, "System Queue initialization Failed!"); 
        return false; 
    }
    ESP_LOGI(SYSTEM_STATUS.TAG, "Initialization of system Queue complete");
    return true; 
}


static void sntp_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    (void)arg; (void)base; (void)id;
    const esp_netif_sntp_time_sync_t *evt = (const esp_netif_sntp_time_sync_t *)data;
    if (evt) {
        char ts[64];
        time_t t = evt->tv.tv_sec;
        struct tm tm_utc;
        gmtime_r(&t, &tm_utc);
        strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", &tm_utc);
        ESP_LOGI("SNTP HANDLER", "SNTP event: time synced (UTC): %s.%06ld", ts, (long)evt->tv.tv_usec);
    } else {
        ESP_LOGI("SNTP HANDLER", "SNTP event: time synced (no timeval provided)");
    }
}



bool init_sntp()
{
    ESP_ERROR_CHECK(esp_event_handler_register(NETIF_SNTP_EVENT, NETIF_SNTP_TIME_SYNC, &sntp_event_handler, NULL));

    ESPLOGI("INIT SNTP", "Initializing the SNTP service"); 
    ESPLOGI("Init SNTP", "System time in UTC0\r\n"); 
    setenv("TZ","UTC0", 1); 
    tzset(); 

   
    
    sntp_setoperatingmode(ESP_SNTP_OPMODE_POLL);
    sntp_setservername(0, "time.google.com");
    sntp_setservername(1, "pool.ntp.com");
    esp_sntp_init();

}

char* getLocalTime()
{
    char strftime_buf[64]; 
    time_t now; 
    struct tm timeinfo; 

    localtime_rr(&now, &timeinfo); 
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo); 
    ESPLOGI("LocalTime", "System local time is %s", strftime_buf); 
    return strftime_buf; 
}


bool systemSemaphoreInit()
{
    SemaphoreHandle_t xSemaphore_systemStatus = vSemaphoreCreateBinary( xSemaphore_systemStatus );
    SemaphoreHandle_t xSemaphore_systemAlarm = vSemaphoreCreateBinary( xSemaphore_systemStatus );
    SemaphoreHandle_t xSemaphore_systemCommChannel = vSemaphoreCreateBinary( xSemaphore_systemCommChannel );
    if(xSemaphore_systemStatus == NULL || xSemaphore_systemAlarm == NULL || xSemaphore_systemCommChannel == NULL)
    {
        ESPLOGE("Semaphore Creation", "Failed to create semaphores "); 
        return false; 
    }
    ESPLOGI("Semaphore Creation", "Completed Initialization of Semaphores"); 
    return true; 
}
bool systemInit()
{
    ESP_ERROR_CHECK( nvs_flash_init()); 
    ESP_ERROR_CHECK(esp_net_if_init()); 
    //create system event loop allow for posting and receiving of events 
    ESP_ERROR_CHECK(esp_event_loop_create_default()); 
    //Start system initialization with creation of Queues 
    ESPLOGI("System Init", "Initialized nvs_flash, esp_net and event loop"); 

    if(!systemQueueInit())
    {
        ESPLOGE("System Init", "Failed to initialize system Queues"); 
        return false; 
    }
    if(!systemSemaphoreInit())
    {
        ESPLOGE("System Init", "Failed to initialize system Semaphores"); 
        return false; 
    } 

}

