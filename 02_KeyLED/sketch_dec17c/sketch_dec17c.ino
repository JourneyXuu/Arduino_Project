/*
按键BOOT0 板载LED48
按下按键 LED48状态取反一次
*/

//外设GPIO端口宏
const int ledpin = 48;
const int Key = 0;

bool status = false; 

void setup() {
  // put your setup code here, to run once:
  pinMode(ledpin,OUTPUT);
  pinMode(Key,INPUT_PULLUP);
  digitalWrite(ledpin,status);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(Key) == 0)
  {
    delay(100);
    if(digitalRead(Key) == 0)
    {
      while(digitalRead(Key) == 0);
      status = !status;
      digitalWrite(ledpin,status);
    }
  }
}
