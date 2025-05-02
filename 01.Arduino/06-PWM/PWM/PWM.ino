//  // 使用Arduino 自带的 analogWrite(pin, value) 函数方式：

// // 宏定义 GPIO 输出引脚
// #define LED_PIN   48

// void setup() 
// {
//   // 配置 GPIO 输出引脚
//   pinMode(LED_PIN, OUTPUT);
// }

// void loop() 
// {
//   // 实现渐亮效果
//   for(int i=0;i<256;i++) 
//   {
//     // 设置亮度模拟值，占空比不断加大
//     analogWrite(LED_PIN, i);
//     // 延时 10ms
//     delay(10);
//     }
//   // 实现渐灭效果
//   for(int i=255;i>=0;i--) 
//   {
//     // 设置亮度模拟值，占空比不断减少
//     analogWrite(LED_PIN, i);
//     // 延时 10ms
//     delay(10);
//     }
// }



#define FREQ        2000    // 频率
#define CHANNEL     0       // 通道
#define RESOLUTION  8       // 分辨率
#define LED         48      // LED 引脚

void setup()
{
  ledcSetup(CHANNEL, FREQ, RESOLUTION); // 设置通道
  ledcAttachPin(LED, CHANNEL);          // 将通道与对应的引脚连接
}

void loop()
{
  // 逐渐变亮
  for (int i=0;i<pow(2, RESOLUTION); i++)
  {
    ledcWrite(CHANNEL, i); // 输出PWM
    delay(5);
  }

  // 逐渐变暗
  for (int i=pow(2, RESOLUTION)-1;i>=0;i--)
  {
    ledcWrite(CHANNEL, i); // 输出PWM
    delay(5);
  }
}


