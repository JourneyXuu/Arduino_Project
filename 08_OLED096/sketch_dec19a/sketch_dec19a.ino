/* 安装 ESP32 OLED库. 在库管理中搜索 -> ESP8266 and ESP32 OLED driver for SSD1306 displays  */
#include <Wire.h>
#include "SSD1306Wire.h"//导入0.96寸屏幕显示库

// 使用Wire库初始化OLED显示器
SSD1306Wire display(0x3c, 10, 9);   // 三个参数分别是 器件地址, SDA引脚, SCL引脚

//函数宏
void drawLines();     //画线
void drawRect(void);  //画矩形
void fillRect(void);  //画填充矩形
void drawCircle(void);//画圆
void printBuffer(void);//


void setup()
{
  //初始化屏幕
  display.init();
  //设置对比度
  display.setContrast(255);

  //画线测试
  drawLines();
  delay(1000);
  // //清屏
  // display.clear();
  // //画矩形测试
  // drawRect();
  // delay(1000);
  // //清屏
  // display.clear();
  // //画填充矩形测试
  // fillRect();
  // delay(1000);
  // //清屏
  // display.clear();
  // //画圆测试
  // drawCircle();
  // delay(1000);
  // display.clear();
  // //显示文本测试
  // printBuffer();
  // delay(1000);
  // display.clear();
}

void loop() { }

/*代码宏*/

//画线
void drawLines()
{
  for (int16_t i = 0; i < display.getWidth(); i += 4)
  {
    display.drawLine(0, 0, i, display.getHeight() - 1);
    display.display();
    delay(10);
  }
  for (int16_t i = 0; i < display.getHeight(); i += 4)
  {
    display.drawLine(0, 0, display.getWidth() - 1, i);
    display.display();
    delay(10);
  }
  delay(250);

  display.clear();
  for (int16_t i = 0; i < display.getWidth(); i += 4)
  {
    display.drawLine(0, display.getHeight() - 1, i, 0);
    display.display();
    delay(10);
  }
  for (int16_t i = display.getHeight() - 1; i >= 0; i -= 4)
  {
    display.drawLine(0, display.getHeight() - 1, display.getWidth() - 1, i);
    display.display();
    delay(10);
  }
  delay(250);

  display.clear();
  for (int16_t i = display.getWidth() - 1; i >= 0; i -= 4)
  {
    display.drawLine(display.getWidth() - 1, display.getHeight() - 1, i, 0);
    display.display();
    delay(10);
  }
  for (int16_t i = display.getHeight() - 1; i >= 0; i -= 4)
  {
    display.drawLine(display.getWidth() - 1, display.getHeight() - 1, 0, i);
    display.display();
    delay(10);
  }
  delay(250);
  display.clear();
  for (int16_t i = 0; i < display.getHeight(); i += 4)
  {
    display.drawLine(display.getWidth() - 1, 0, 0, i);
    display.display();
    delay(10);
  }
  for (int16_t i = 0; i < display.getWidth(); i += 4)
  {
    display.drawLine(display.getWidth() - 1, 0, i, display.getHeight() - 1);
    display.display();
    delay(10);
  }
  delay(250);
}

//画矩形
void drawRect(void)
{
  for (int16_t i = 0; i < display.getHeight() / 2; i += 2)
  {
    display.drawRect(i, i, display.getWidth() - 2 * i, display.getHeight() - 2 * i);
    display.display();
    delay(10);
  }
}

//画填充矩形
void fillRect(void)
{
  uint8_t color = 1;
  for (int16_t i = 0; i < display.getHeight() / 2; i += 3)
  {
    display.setColor((color % 2 == 0) ? BLACK : WHITE); // alternate colors
    display.fillRect(i, i, display.getWidth() - i * 2, display.getHeight() - i * 2);
    display.display();
    delay(10);
    color++;
  }
  // Reset back to WHITE
  display.setColor(WHITE);
}

// 画圆
void drawCircle(void)
{
  for (int16_t i = 0; i < display.getHeight(); i += 2)
  {
    display.drawCircle(display.getWidth() / 2, display.getHeight() / 2, i);
    display.display();
    delay(10);
  }
  delay(1000);
  display.clear();

  display.drawCircleQuads(display.getWidth() / 2, display.getHeight() / 2, display.getHeight() / 4, 0b00000001);
  display.display();
  delay(200);
  display.drawCircleQuads(display.getWidth() / 2, display.getHeight() / 2, display.getHeight() / 4, 0b00000011);
  display.display();
  delay(200);
  display.drawCircleQuads(display.getWidth() / 2, display.getHeight() / 2, display.getHeight() / 4, 0b00000111);
  display.display();
  delay(200);
  display.drawCircleQuads(display.getWidth() / 2, display.getHeight() / 2, display.getHeight() / 4, 0b00001111);
  display.display();
}

void printBuffer(void)
{
  // 初始化日志缓冲区
  // 分配内存来存储8行文本，每行30个字符。
  display.setLogBuffer(5, 30);

  // 测试内容
  const char* test[] =
  {
    "Hello",
    "World" ,
    "----",
    "Show off",
    "how",
    "the log buffer",
    "is",
    "working.",
    "Even",
    "scrolling is",
    "working"
  };

  for (uint8_t i = 0; i < 11; i++)
  {
    //清屏
    display.clear();
    //打印到屏幕并且换行
    display.println(test[i]);
    // 将其绘制到内部屏幕缓冲区
    display.drawLogBuffer(0, 0);
    // 把它显示在屏幕上
    display.display();
    delay(500);
  }
}

