/***
 * System level definitions for the 
 * 
 */


#include "definitions.h"

//JSON lib
#include <cjson/cJSON.h>

struct CONFIG_MQTT {
    char* MQTT_ADDRESS; 
    char* MQTT_
}; 

struct 


/**
 * @brief function passes a string and returns JSON file 
 * @param json - pass the string of the JSON object 
 * @return cJson - the json string is parsed and a JSON object is returned 
 * 
 */
cJSON* parse_string(const char* json); 


/**
 * @brief extract value string from JSON object 
 * @param json - pointer to the json object 
 *        objname - Key of the JSON object
 * 
 * @return returns constant reference to the string of the value of the JSON object * 
 */
const char* getJSONObjectItem(cJSON* json, const char* objname); 

