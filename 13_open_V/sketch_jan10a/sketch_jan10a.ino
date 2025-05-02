

#define LED   2
#define m1_pwma 17
#define m1_pwmb 5
#define m1_pwmc 18
#define m1_en 19

#define _constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))


     `hw_timer_t *timer = NULL;
void timer_interrupt()
{
  //修改LED的状态，如果亮则修改为灭；如果灭则修改为亮
  digitalWrite(LED, !digitalRead(LED));
}

void setup()
{
  pinMode(LED, OUTPUT);
  timer = timerBegin(1000000);
  timerAttachInterrupt(timer,timer_interrupt);
  timerAlarm(timer,1000000,true, 0);

  Serial.begin(115200);
  pinMode(m1_en,OUTPUT); 

  ledcAttach(m1_pwma,30000,8);
  ledcAttach(m1_pwmb,30000,8);
  ledcAttach(m1_pwmc,30000,8);

  Serial.println("完成PWM初始化设置");
  delay(3000);

}

void loop()
{
  ledcWrite(m1_pwma, 50);
  ledcWrite(m1_pwmb, 50);
  ledcWrite(m1_pwmc, 50);
}