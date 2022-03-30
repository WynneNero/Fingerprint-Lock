#include "AS608.h"
#include "oled.h"
#include "EEPROM.h"


People people[NUMBER_OF_PRINTER];

#define mySerial Serial2 
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial); 
Lock_Operation LockOper = {
  .Delete = -1,
  .Add = -1,
  };
  
void taskGingerprintLock(void* parameter)
{   
    EEPROM.begin(4096);
    for(int i=0; i<NUMBER_OF_PRINTER; i++){
      EEPROM.get(i * sizeof(People), people[i]);
    }
    
    initFingerprint();
    pinMode(BUTTON_PIN, INPUT);
    while(1){
        vTaskDelay(100);
        if(LockOper.Add != -1){
          setOledMsg("开始新增指纹");
            for(int i=0; i<NUMBER_OF_PRINTER; i++){
                if(people[i].Status != 1){
                    if(getFingerprintEnroll(i) == true){
                        addPeople(i, LockOper.Name);
                        setOledMsg("新增指纹成功");                      
                    }
                    else setOledMsg("新增指纹失败");
                    break; 
                }
            }
            LockOper.Add = -1;
        }
        if(LockOper.Delete != -1){
            if(deleteFingerprint(LockOper.Delete) == FINGERPRINT_OK){
                deletePeople(LockOper.Delete);
                setOledMsg("删除指纹成功"); 
            }  else setOledMsg("删除指纹失败");  
            LockOper.Delete = -1;
        }
        
        if(digitalRead(STATUS_PIN)){
            vTaskDelay(100);
            if(digitalRead(STATUS_PIN)){
                int id = getFingerprintIDez();
                if(id != -1){
                    Serial1.println("open");
                    setOledMsg("欢迎基佬: " + String(id));
                    setOledMsg(String(people[id].Name));
                    vTaskDelay(100);
                    uint64_t t = millis();
                    while(digitalRead(STATUS_PIN) && (millis() - t) < 4000)
                        vTaskDelay(10);
                }else setOledMsg("匹配错误");
            }    
        }
        
        if(digitalRead(BUTTON_PIN)){
            vTaskDelay(100);
            if(digitalRead(BUTTON_PIN)){
                setOledMsg("关门");
                Serial1.println("close");              
              }
        } 
         
    }
}

void initFingerprint(void){
    finger.begin(57600); 
    pinMode(STATUS_PIN, INPUT);
    while(!finger.verifyPassword()) {
        setOledMsg("没有发现设备");
    }
    setOledMsg("欢迎使用!!!");

//    Serial.println(F("Reading sensor parameters")); 
    finger.getParameters(); 
//    Serial.print(F("Status: 0x")); 
//    Serial.println(finger.status_reg, HEX); 
//    Serial.print(F("Sys ID: 0x")); 
//    Serial.println(finger.system_id, HEX); 
//    Serial.print(F("Capacity: ")); 
//    Serial.println(finger.capacity); 
//    Serial.print(F("Security level: ")); 
//    Serial.println(finger.security_level); 
//    Serial.print(F("Device address: ")); 
//    Serial.println(finger.device_addr, HEX); 
//    Serial.print(F("Packet len: ")); 
//    Serial.println(finger.packet_len); 
//    Serial.print(F("Baud rate: ")); 
//    Serial.println(finger.baud_rate);   
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez(void) {
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK)  return -1;
    
    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)  return -1;
  
    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)  return -1;

    return finger.fingerID;
} 
 
uint8_t deleteFingerprint(uint8_t id) {
    uint8_t p = -1;
    
    p = finger.deleteModel(id);
    
    if (p == FINGERPRINT_OK) {
        setOledMsg("成功删除ID:" + String(id));
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
        setOledMsg("通信错误");
    } else if (p == FINGERPRINT_BADLOCATION) {
        setOledMsg("无法删除");
    } else if (p == FINGERPRINT_FLASHERR) {
        setOledMsg("数据写入错误");
    } else {
        setOledMsg("未知错误:" + String(p)); 
    }
    return p;
}

uint8_t getFingerprintEnroll(uint8_t id) 
{ 
    int p = -1;    
    uint64_t exit_time = millis();
    setOledMsg("请按下手指");
    while (p != FINGERPRINT_OK) 
    { 
        if(millis() - exit_time > EXIT_TIME) return 0;
        p = finger.getImage(); 
        switch (p) { 
            case FINGERPRINT_OK: setOledMsg("获得指纹图像"); break; 
            case FINGERPRINT_NOFINGER: /*setOledMsg("请按下手指");*/  break; 
            case FINGERPRINT_PACKETRECIEVEERR: setOledMsg("通信错误"); break; 
            case FINGERPRINT_IMAGEFAIL: setOledMsg("图像错误"); break; 
            default: setOledMsg("未知错误"); break; 
        } 
    }
    vTaskDelay(2000); 
    // OK success! 
    p = finger.image2Tz(1); 
    switch (p) { 
        case FINGERPRINT_OK: setOledMsg("图像转换完成"); break; 
        case FINGERPRINT_IMAGEMESS: setOledMsg("通信错误"); return p; 
        case FINGERPRINT_PACKETRECIEVEERR: setOledMsg("通信错误"); return p; 
        case FINGERPRINT_FEATUREFAIL: setOledMsg("没有发现特征"); return p; 
        case FINGERPRINT_INVALIDIMAGE: setOledMsg("没有发现特征"); return p; 
        default: setOledMsg("未知错误"); return p; 
    }
    vTaskDelay(2000);  
    setOledMsg("请移开手指"); 
    vTaskDelay(2000); 
    p = 0;
    exit_time = millis(); 
    while (p != FINGERPRINT_NOFINGER) 
    { 
        if(millis() - exit_time > EXIT_TIME) return 0;
        p = finger.getImage(); 
    } 
    setOledMsg("请再按一次"); 
    exit_time = millis(); 
    while (p != FINGERPRINT_OK) 
    { 
        if(millis() - exit_time > EXIT_TIME) return 0;
        p = finger.getImage(); 
        switch (p) { 
            case FINGERPRINT_OK: setOledMsg("获得指纹图像"); break; 
            case FINGERPRINT_NOFINGER: /*setOledMsg("Please push");*/ break; 
            case FINGERPRINT_PACKETRECIEVEERR: setOledMsg("通信错误"); break; 
            case FINGERPRINT_IMAGEFAIL: setOledMsg("图像错误"); break; 
            default: setOledMsg("未知错误"); break; 
        } 
    } // OK success! 
    p = finger.image2Tz(2); 
    vTaskDelay(2000); 
    switch (p) { 
        case FINGERPRINT_OK: setOledMsg("图像转换完成"); break; 
        case FINGERPRINT_IMAGEMESS: setOledMsg("Image too messy"); return p; 
        case FINGERPRINT_PACKETRECIEVEERR: setOledMsg("Commun error"); return p; 
        case FINGERPRINT_FEATUREFAIL: setOledMsg("not find features"); return p; 
        case FINGERPRINT_INVALIDIMAGE: setOledMsg("not find features"); return p; 
        default: setOledMsg("Unknown error"); return p; 
    } // OK converted! 
    
    setOledMsg("为" + String(id) + "创建模型");
    p = finger.createModel(); 
    if (p == FINGERPRINT_OK) { 
        setOledMsg("指纹匹配"); 
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) { 
        setOledMsg("通信错误"); 
        return p; 
    } else if (p == FINGERPRINT_ENROLLMISMATCH) { 
        setOledMsg("无法匹配"); 
        return p; 
    } else { 
        setOledMsg("未知错误"); 
        return p; 
    } 
    vTaskDelay(2000);
    p = finger.storeModel(id); 
    if (p == FINGERPRINT_OK) { 
        setOledMsg("数据写入成功"); 
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) { 
        setOledMsg("通信错误"); 
        return p; 
    } else if (p == FINGERPRINT_BADLOCATION) { 
        setOledMsg("无法写入数据");
        return p; 
    } else if (p == FINGERPRINT_FLASHERR) { 
        setOledMsg("数据写入失败"); 
        return p; 
    } else { 
        setOledMsg("未知错误"); 
        return p; 
    }  
    return true;  
}
void deletePeople(uint8_t id){
    if(id >= NUMBER_OF_PRINTER) {
      setOledMsg("ID范围错误");
      return;
    }
    people[id].Status = 0;
    memset(people[id].Name, 0x00, LEN_OF_NAME); 
    EEPROM.put(id * sizeof(People), people[id]); 
    EEPROM.commit();
}
void addPeople(uint8_t id, String str){
    if(id >= NUMBER_OF_PRINTER) {
      setOledMsg("ID范围错误");
      return;
    }
    people[id].Status = 1;
    int len = str.length();
    if(len >= LEN_OF_NAME) len = LEN_OF_NAME;
    for(int i=0; i<len; i++)
      people[id].Name[i] = str[i]; 
    EEPROM.put(id * sizeof(People), people[id]); 
    EEPROM.commit();
}
