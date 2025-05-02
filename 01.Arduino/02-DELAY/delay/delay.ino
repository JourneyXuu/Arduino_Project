void setup() {
  // 设置 GPIO48 Pin 为输出模式
  pinMode(48,OUTPUT);
  // 设置48引脚 输出低电平
  digitalWrite(48,LOW);
}

void loop() {
  // 设置48引脚 输出低电平
  digitalWrite(48,LOW);
  //延时100ms
  delay(100);
  // 设置48引脚 输出高电平
  digitalWrite(48,HIGH);
  //延时100ms
  delay(100);
}
