#include "AS608.h"
#include "oled.h"

void taskWeb(void* parameter);

void setup() { 
    Serial.begin(115200); 
    Serial1.begin(115200, SERIAL_8N1, 18, 19);
    delay(100);
    
    xTaskCreatePinnedToCore(taskWeb,          /* Task function. */
            "taskWeb",        /* String with name of task. */
            10000,            /* Stack size in bytes. */
            NULL,             /* Parameter passed as input of the task */
            5,                /* Priority of the task. */
            NULL, 0);         /* Task handle. */       
    xTaskCreatePinnedToCore(taskGingerprintLock,          /* Task function. */
            "taskGingerprintLock",        /* String with name of task. */
            10000,            /* Stack size in bytes. */
            NULL,             /* Parameter passed as input of the task */
            2,                /* Priority of the task. */
            NULL, 1);         /* Task handle. */ 
    xTaskCreatePinnedToCore(taskOledDisplay,          /* Task function. */
            "taskOledDisplay",        /* String with name of task. */
            10000,            /* Stack size in bytes. */
            NULL,             /* Parameter passed as input of the task */
            2,                /* Priority of the task. */
            NULL, 1);         /* Task handle. */ 
               
} 
void loop() { // run over and over again 
 
} 
