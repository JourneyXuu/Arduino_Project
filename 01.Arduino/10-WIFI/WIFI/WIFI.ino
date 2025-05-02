#include <WiFi.h>
#define LED   48

// 定义 Wi-Fi 名与密码
const char * ssid = "4G-UFI-7787";
const char * password = "1234567890";

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


// #include <WiFi.h>

// #define LED 48

// // 设置要创建的热点名与密码
// const char * ssid = "LCKFB_ESP32";
// const char * password = "12345678";


// void setup() {
//   Serial.begin(9600);

//   // 创建热点
//   WiFi.softAP(ssid, password);

//   //  打印热点 IP
//   Serial.print("Wi-Fi 接入的 IP：");
//   Serial.println(WiFi.softAPIP());


//   // 使用板载 LED 反馈创建成功
//   pinMode(LED, OUTPUT);
// }

// void loop() {
//   //LED闪烁说明创建热点成功
//   digitalWrite(LED, HIGH);
//   delay(500);
//   digitalWrite(LED, LOW);
//   delay(500);
// }

