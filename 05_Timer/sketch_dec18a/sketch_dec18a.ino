#define LED   48

//定义一个定时器对象
hw_timer_t *timer = NULL;

// 定时器中断处理函数
void timer_interrupt()
{
  //修改LED的状态，如果亮则修改为灭；如果灭则修改为亮
  digitalWrite(LED, !digitalRead(LED));
}

void setup()
{
  //设置LED引脚（48） 为输出模式
  pinMode(LED, OUTPUT);

  // 初始化定时器频率为1MHz
  timer = timerBegin(1000000);

  // 配置定时器中断服务函数
  //定时器0的中断回调函数为timer_interrupt()
  timerAttachInterrupt(timer,timer_interrupt);

  // 设置定时器的计数值
  // 定时器的计数值为1000,000，true为允许自动重载计数值
  // 计数值单位为us,1000,000 us = 1000ms = 1s
  timerAlarm(timer,1000000,true, 0);
}

void loop()
{

}