//IO 宏定义
#define Button 0 
#define LED 48

//状态布尔定义
bool led_status = false;
volatile bool Button_status = false;   //中断回调函数与主程序共享的变量，violate,确保变量不被编译器优化为缓存读取，每次读取从内存读取

//中断函数声明
void button_ISR();

void setup() {
  // put your setup code here, to run once:
  pinMode(Button,INPUT_PULLUP);
  pinMode(LED,OUTPUT);

  attachInterrupt(digitalPinToInterrupt(Button),button_ISR,FALLING); //配置中断引脚为GPIO0，外部中断回调函数为 ISR(), 中断下降沿触发
}


void loop() {
  // put your main code here, to run repeatedly:
  
  // if(Button_status == 1)
  // {
  //   led_status = !led_status;
  //   digitalWrite(LED,led_status);
    
  //   Button_status = 0;
  // }

}

//按键中断服务函数
void button_ISR()
{
    led_status = !led_status;
    digitalWrite(LED,led_status);
}


