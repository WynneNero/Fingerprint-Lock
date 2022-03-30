#include "arduino.h"
#include <Adafruit_Fingerprint.h> 

#define NUMBER_OF_PRINTER  100
#define LEN_OF_NAME  16 
#define STATUS_PIN  4 
#define BUTTON_PIN  12 
#define EXIT_TIME   5000  //ms 

typedef struct Lock_Operation{
    int Delete;
    int Add;
    String Name;
};
typedef struct People{
    int Status; 
    char Name[LEN_OF_NAME];
};
extern Lock_Operation LockOper;

void taskGingerprintLock(void* parameter);
void initFingerprint(void);
uint8_t getFingerprintEnroll(uint8_t id);
uint8_t deleteFingerprint(uint8_t id);
uint8_t getFingerprintID(void);

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez(void);

void deletePeople(uint8_t id);
void addPeople(uint8_t id, String str);
