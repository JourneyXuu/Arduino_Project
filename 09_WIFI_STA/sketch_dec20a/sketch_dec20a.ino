#include <WiFi.h>
#define LED 48

// 定义 要连接的 Wi-Fi 名与密码
const char* ssid = "ikun";
const char* password = "2nianban";

void setup() {
Serial.begin(9600);

// 断开之前的连接
WiFi.disconnect(true);
// 连接 Wi-Fi
WiFi.begin(ssid, password);

Serial.print("正在连接 Wi-Fi");

// 检测是否链接成功
while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
}
Serial.println("连接成功");
Serial.print("IP address: ");
Serial.println(WiFi.localIP());

// 使用板载 LED 反馈连接成功
pinMode(LED, OUTPUT);

}

void loop() {

      //LED闪烁说明连接成功
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      delay(500);
}