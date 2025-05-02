
#include <math.h> // 引入数学库，以便使用 pow 函数

//宏
#define Resolution 9     //分辨率
//IO宏
#define LED 48
#define Lin1 15
#define Lin2 16
#define Rin1 18
#define Rin2 17

//函数宏
void motor_init();
void motor_left();
void motor_right();
void motor_forward();
void motor_stop();
void Timer_Handler(void);
//对象
hw_timer_t *Timer = NULL;

void setup() {
  // put your setup code here, to run once:
  motor_init();
  pinMode(LED,OUTPUT);

  Timer = timerBegin(1000000);    // 初始化定时器频率为1MHz
  timerAttachInterrupt(Timer,Timer_Handler);  //定时器0的中断回调函数为Timer_Handler
  timerAlarm(Timer,1000000,true,0);           //计数值为 1000 000，意为1s进一次中断，用来闪灯

  // motor_forward();

  // motor_left();

  // motor_right();

  motor_back();
 
  delay(10000);
  motor_stop();
}

void loop() {
  // put your main code here, to run repeatedly:

}


void Timer_Handler(void)
{
  digitalWrite(LED, !digitalRead(LED));
}
/**************************底盘******************************/
void motor_init()
{
    //L_IN1
    ledcAttach(Lin1,10000,Resolution);   
    //L_IN2
    ledcAttach(Lin2,10000,Resolution);   
    //R_IN1
    ledcAttach(Rin1,10000,Resolution);         
    //R_IN2
    ledcAttach(Rin2,10000,Resolution);   
    motor_stop();  
}

void motor_left(void)
{
  ledcWrite(Lin1, 340); // 左
  ledcWrite(Lin2, 0);

  ledcWrite(Rin1, 400); // 右
  ledcWrite(Rin2, 0); 
}
void motor_right(void)
{
  ledcWrite(Lin1, 400); // 左
  ledcWrite(Lin2, 0);

  ledcWrite(Rin1, 340); // 右
  ledcWrite(Rin2, 0); 
}
// speed range from 0-100
void motor_forward(void)
{
  ledcWrite(Lin1, 400); // 左
  ledcWrite(Lin2, 0);

  ledcWrite(Rin1, 400); // 右
  ledcWrite(Rin2, 0); 
}

void motor_back(void)
{

  ledcWrite(Lin1, 0); // 左
  ledcWrite(Lin2, 400);

  ledcWrite(Rin1, 0); // 右
  ledcWrite(Rin2, 400); 
}
void motor_stop()
{
  ledcWrite(Lin1, 0); // 输出PWM
  ledcWrite(Lin2, 0); // 输出PWM
  ledcWrite(Rin1, 0); // 输出PWM
  ledcWrite(Rin2, 0); // 输出PWM
}

