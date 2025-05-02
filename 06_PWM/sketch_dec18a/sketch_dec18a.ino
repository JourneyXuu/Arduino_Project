// 使用Arduino 自带的 analogWrite(pin, value) 函数方式：

// 宏定义 GPIO 输出引脚
#define LED_PIN 48

void setup()
{
      pinMode(LED_PIN, OUTPUT);
      // 配置 GPIO 输出引脚

}

void loop()
{
      // 实现渐亮效果
      for(int i=0;i<256;i++)
      {
            // 设置亮度模拟值，占空比不断加大
            analogWrite(LED_PIN, i);
            // 延时 10ms
            delay(10);
      }
      // 实现渐灭效果
      for(int i=255;i>=0;i--)
      {
            // 设置亮度模拟值，占空比不断减少
            analogWrite(LED_PIN, i);
            // 延时 10ms
            delay(10);
      }
}