#ifndef OLED_H
#define OLED_H
#include <Wire.h>
#include <U8g2lib.h>

typedef struct Oled_Msg{
    String Msg1;
    String Msg2;
    String Msg3;
    String Msg4;
    uint32_t Time;
};
extern Oled_Msg oled_msg;
void setOledMsg(String msg);

void taskOledDisplay(void* parameter);
void oledIpDisplay(String Ip, String msg1, String msg2);

#endif
