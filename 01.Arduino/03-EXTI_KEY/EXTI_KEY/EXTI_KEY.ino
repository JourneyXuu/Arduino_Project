
#define BUTTON    0
#define LED       48 

//LED状态
bool led_flag = false;
//按键状态（中断回调函数与主程序共享的变量要加上 volatile 关键字）
volatile bool flag = false;

//中断回调函数
void ISR() 
{
  flag = true;
}

void setup() 
{
  //设置KEY引脚（0） 为上拉输入模式
  pinMode(BUTTON, INPUT_PULLUP);

  //设置LED引脚（48） 为输出模式
  pinMode(LED, OUTPUT);

  // 配置中断引脚为GPIO0，外部中断回调函数为 ISR(), 中断下降沿触发
  attachInterrupt(digitalPinToInterrupt(BUTTON), ISR, FALLING);
}

void loop() 
{  
  //当按键按下时会产生一个下降沿，进入回调函数 ISR()，而ISR()中只是将flag = true
  //故当flag == true 时，说明按键按下
  if ( flag == true ) {

    //延时200毫秒
    delay(200);
    //LED状态取反
    digitalWrite(LED, led_flag=!led_flag);

    // 重置中断标志位
    flag = false;
  }
}
