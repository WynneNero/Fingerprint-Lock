#include "oled.h"
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 14, /* data=*/ 15);
Oled_Msg oled_msg;

void oledDisplay(void)
{
    u8g2.clearBuffer();
    if(WiFi.status() == WL_CONNECTED){
        u8g2.setFont(u8g2_font_unifont_t_chinese2);
        u8g2.setCursor(0, 14);
        u8g2.print("IP:" + WiFi.localIP().toString());      
    }else{
        u8g2.setFont(u8g2_font_wqy12_t_gb2312);
        u8g2.setCursor(0, 14);
        u8g2.print("未连接网络"); 
    }
    u8g2.setFont(u8g2_font_wqy12_t_gb2312);
    u8g2.setCursor(0, 30);
    u8g2.print(oled_msg.Msg2); 
    u8g2.setCursor(0, 44);
    u8g2.print(oled_msg.Msg3); 
    u8g2.setCursor(0, 61);
    u8g2.print(oled_msg.Msg4);
    u8g2.sendBuffer();  
}

void taskOledDisplay(void* parameter)
{   
    u8g2.begin();
    u8g2.enableUTF8Print();
    while(1){
        vTaskDelay(100);
        if(millis() - oled_msg.Time > 4000){
            oled_msg.Msg1 = oled_msg.Msg2;
            oled_msg.Msg2 = oled_msg.Msg3;
            oled_msg.Msg3 = oled_msg.Msg4;
            oled_msg.Msg4 = "";
            oled_msg.Time = millis(); 
        }
        oledDisplay();
    }
}
void setOledMsg(String msg)
{
    oled_msg.Msg1 = oled_msg.Msg2;
    oled_msg.Msg2 = oled_msg.Msg3;
    oled_msg.Msg3 = oled_msg.Msg4;
    oled_msg.Msg4 = msg;
    oled_msg.Time = millis(); 
  }
void oledSetupDisplay(void)
{
    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_unifont_t_chinese2);
    u8g2.setCursor(0, 14);
    u8g2.print("Scanning for wifi");
    u8g2.setCursor(0, 30);
    u8g2.print("Please wait...");
    u8g2.setCursor(0, 47);
    u8g2.print("Designed By ");
    u8g2.setCursor(90, 64);
    u8g2.print("Tajor");
    u8g2.sendBuffer();
}
void oledIpDisplay(String Ip, String msg1, String msg2)
{
    u8g2.clearBuffer();
    if(WiFi.status() != WL_CONNECTED){
        u8g2.setFont(u8g2_font_unifont_t_chinese2);
        u8g2.setCursor(0, 14);
        u8g2.print("using ip to link");
        u8g2.setCursor(0, 30);
        u8g2.print(Ip);      
    }else{
        u8g2.setFont(u8g2_font_wqy12_t_gb2312);
        u8g2.setCursor(0, 14);
        u8g2.print("no net"); 
    }
    u8g2.setCursor(0, 44);
    u8g2.print(msg1); 
    u8g2.setCursor(0, 61);
    u8g2.print(msg2);
    u8g2.sendBuffer();  
}
