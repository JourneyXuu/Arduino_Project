const int led_pin = 2;
unsigned long previousMillis = 0;
unsigned long interval = 1000;
bool ledState = LOW;

//只执行1次
void setup()
{
  // 设置 GPIO48 引脚 为输出模式
  pinMode(led_pin,OUTPUT);
} 
//循环执行
void loop()
{
  unsigned long currentMillies = millis();
  if( currentMillies - previousMillis >= interval)
  {
    previousMillis = currentMillies;
    ledState = !ledState;
    digitalWrite(led_pin,ledState);
  }


/*阻塞式延时
  digitalWrite(led_pin,LOW);
  delay(1000);
  digitalWrite(led_pin,HIGH);
  delay(1000); 
*/
}