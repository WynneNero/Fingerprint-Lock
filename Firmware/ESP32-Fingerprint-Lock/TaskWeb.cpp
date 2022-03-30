#include "arduino.h"
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "AS608.h"
#include "oled.h"


const char* ssid     = "yourwifi";
const char* password = "password";

void taskWeb(void* parameter)
{   
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    int link_time = 200; 
    while (WiFi.status() != WL_CONNECTED && link_time--) {
        vTaskDelay(100);
        Serial.print(".");
    }
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
      
    WiFiServer server(80);
    String header;
    unsigned long currentTime = millis();
    unsigned long previousTime = 0;
    const long timeoutTime = 2000;
    server.begin();
    while(1){
      vTaskDelay(1); 
      WiFiClient client = server.available();   // Listen for incoming clients
      if (client) {                             // If a new client connects,
          currentTime = millis();
          previousTime = currentTime;
          String currentLine = "";                // make a String to hold incoming data from the client
          while (client.connected() && currentTime - previousTime <= timeoutTime) {            // loop while the client's connected
              currentTime = millis();
              if (client.available()) {             // if there's bytes to read from the client,
                  char c = client.read();             // read a byte, then
                  header += c;
                  if (c == '\n') {                    // if the byte is a newline character
                      if (currentLine.length() == 0) {
                          client.println("HTTP/1.1 200 OK");
                          client.println("Content-type:text/html");
                          client.println("Connection: close");
                          client.println();
                          // Display the HTML web page
                          client.println("<!DOCTYPE html><html>");
                          client.println("<head><center><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                          client.println("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />");
                          
                          client.println("</head><body><div class=\"container\"><div class=\"row\"><h1>指纹锁管理</h1></div>");
                          client.println("<br />");
                          
                          client.println("<p><a href=\"button_open\"><button class=\"button button2\">点击打开指纹锁</button></a></p>");
                          client.println("<p><a href=\"button_close\"><button class=\"button button2\">点击关闭指纹锁</button></a></p>");
            
                          client.println("<center><input id=\"del_id_value\" type=\"text\" name=\"del\" placeholder=\"输入需要删除的ID（1~200）\"/>");
                          client.println("<button><a id=\"del_fingerprint\" role=\"button\" onclick=\"del()\">删除指纹</a></button> ");
                          client.println("<script>function del() {document.getElementById(\"del_fingerprint\").href=\"del&\" + document.getElementById(\"del_id_value\").value + \"*\";}</script>");
                          client.println("<br /><br />");
                          client.println("<input id=\"add_id_value\" type=\"text\" name=\"add\" placeholder=\"输入名字(英语或拼音)\"/>");
                          client.println("<button><a id=\"add_fingerprint\" role=\"button\" onclick=\"add()\">添加指纹</a></button> ");
                          client.println("<script>function add() {document.getElementById(\"add_fingerprint\").href=\"add&\" + document.getElementById(\"add_id_value\").value + \"*\";}</script>");
                          client.println("<script>function open() {document.getElementById(\"open\").href=\"open&\" + document.getElementById(\"add_id_value\").value + \"*\";}</script>");
                          client.println("<script>function close() {document.getElementById(\"close\").href=\"close&\" + document.getElementById(\"add_id_value\").value + \"*\";}</script>");
                          client.println("</center></body></html>");
                          // The HTTP response ends with another blank line
                          client.println();
             
                          //Serial.println(header);
                          if (header.indexOf("GET /del&") >= 0) {
                              String sta;
                              int pos1 = header.indexOf('&');
                              int pos2 = header.indexOf('*');
                              String id = header.substring(pos1 + 1, pos2);
                              int ID = id.toInt();
                              if(ID > 0 && ID < NUMBER_OF_PRINTER){
                                  sta = "</head><body><div class=\"container\"><div class=\"row\"><h1>成功删除指纹ID：";
                                  sta += id;
                                  sta += "</h1></div>";
                                  LockOper.Delete = ID;                                   
                              }else{
                                  sta = "</head><body><div class=\"container\"><div class=\"row\"><h1>输入ID格式有误 ";
                                  sta += "</h1></div>";                                 
                              }
                              client.println(sta);
                          }else if (header.indexOf("GET /add&") >= 0) {
                              String sta;
                              int pos1 = header.indexOf('&');
                              int pos2 = header.indexOf('*');
                              String tmp = header.substring(pos1 + 1, pos2);
                              sta = "</head><body><div class=\"container\"><div class=\"row\"><h1>开始增加指纹";
                              sta += tmp;
                              sta += "</h1></div>"; 
                              LockOper.Name = tmp;
                              LockOper.Add = 1;  
                              client.println(sta);
                          }
                          else if (header.indexOf("button_open") >= 0) {
                              Serial1.println("open");
                              setOledMsg("开锁中...");
//                              String sta;
//                              sta = "</head><body><div class=\"container\"><div class=\"row\"><h1>open";
//                              sta += "</h1></div>"; 
//                              client.println(sta);   
                          }
                          else if (header.indexOf("button_close") >= 0) {
                              Serial1.println("close");
                              setOledMsg("关锁中...");  
//                              String sta;
//                              sta = "</head><body><div class=\"container\"><div class=\"row\"><h1>close";
//                              sta += "</h1></div>"; 
//                              client.println(sta); 
                          }
                          break;
                      } else { // if you got a newline, then clear currentLine
                          currentLine = "";
                      }
                  } else if (c != '\r') {  // if you got anything else but a carriage return character,
                      currentLine += c;    // add it to the end of the currentLine
                  }
              }
          }
          header = "";
          client.stop();
      }
  }
}
