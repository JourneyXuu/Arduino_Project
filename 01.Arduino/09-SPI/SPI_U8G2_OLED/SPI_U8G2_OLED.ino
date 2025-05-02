#include <Arduino.h>
#include <U8g2lib.h>

// 构造对象
U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/8, /* data=*/9, 
                                             /* cs=*/12, /* dc=*/11, /* reset=*/10);

void setup(void)
{
  // 初始化 oled 对象
  u8g2.begin();
  // 开启中文字符集支持
  u8g2.enableUTF8Print();
}

void loop(void)
{
  // 设置字体
  u8g2.setFont(u8g2_font_unifont_t_chinese2);
  // 设置字体方向
  u8g2.setFontDirection(0);

  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.print("Hello LCKFB!");
  u8g2.setCursor(0, 40);
  u8g2.print("你好立创开发板!");
  u8g2.sendBuffer();

  delay(1000);
}
