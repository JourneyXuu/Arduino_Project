#include <WiFi.h>

#define LED 48

// 设置要创建的热点名与密码
const char*  ssid = "LCKFB_ESP32";
const char* password = "12345678";

void setup() {
      Serial.begin(9600);

      // 创建热点
      WiFi.softAP(ssid, password);

      // 打印热点 IP
      Serial.print("Wi-Fi 接入的 IP：");
      Serial.println(WiFi.softAPIP());

      // 使用板载 LED 反馈创建成功
      pinMode(LED, OUTPUT);
}

void loop() {
      //LED闪烁说明创建热点成功
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      delay(500);
}