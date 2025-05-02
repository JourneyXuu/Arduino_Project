// 定义 LED 引脚
int led_pin = 48;
// 定义 按键 引脚
int button_pin = 0;

// 定义 LED 逻辑值
int led_logic = 0;

//定义LED状态变量，默认为假即低电平， 通过它判断 LED 的状态是否改变过
bool status = false;

//只执行一次
void setup() 
{  
  //设置LED引脚（48） 为输出模式
  pinMode(led_pin, OUTPUT);

  //设置按键引脚（0） 为上拉输入模式
  pinMode(button_pin, INPUT_PULLUP);
}

//循环执行
void loop() 
{
    // 如果读取到的按键引脚电平为 0 （低电平）
    // 说明按键按下
    if ( digitalRead(button_pin) == 0 )
    {
        // 延时 100ms，消除按键抖动
        delay(100);
        //如果消抖后按键状态还是低电平0
        //说明不是按键抖动导致的按下
        if (digitalRead(button_pin) == 0 ) 
        {
            //将LED的状态变量进行取反，即在低电平和高电平之间切换
            status = !status;
            //控制LED状态变化
            digitalWrite(led_pin, status);
        }
    }
}
