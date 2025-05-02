#define BLINKER_WIFI
#include <Blinker.h>

//宏
#define Resolution 9     //分辨率
char auth[] = "e6fab95e8ac0";
char ssid[] = "XinXun-8633";
char pswd[] = "12345678";
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
void control(void);

// 按下按键即会执行该函数
void button1_callback(const String & state) {
    digitalWrite(LED, !digitalRead(LED));
}
//对象
BlinkerNumber Number1("num-abc");
BlinkerButton Button1("btn-led");

int counter = 0;
String KeyName;
String KeyValue;


void setup() {
  // put your setup code here, to run once:
  // 初始化串口
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);

  //初始化电机
  motor_init();
  pinMode(LED,OUTPUT);
  digitalWrite(LED, LOW);  

  // 初始化blinker
  Blinker.begin(auth, ssid, pswd);
  Blinker.attachData(dataRead);
  Button1.attach(button1_callback);
}

void loop() {
  // put your main code here, to run repeatedly:
    Blinker.run();
    control();
}


/**************************WIFI链接端******************************/
/*
函数名：dataRead
功能：对发送来的数据做处理,储存键名、键值，日志加一
说明：
    发送过来的数据格式：  
    {"btn-fnt":"tap"}
    {"num-abc":{"val":5}}
    在第一次出现" "，这两个夹着的是按键名KeyName
    后边出现的"",中间夹得是KeyValue
*/
void dataRead(const String & data)
{
  //日志+1
    BLINKER_LOG("Blinker readString: ", data);
    counter++;
    Number1.print(counter);

  //储存键值和键名
    int startPos = data.indexOf('"') + 1;       //提取第一次"出现的索引位置
    int endPos = data.indexOf('"', startPos);   //从第一次"往后第一个出现"的位置
    // 提取双引号内的内容
    KeyName = data.substring(startPos, endPos);
    Serial.println(KeyName);

    startPos = data.indexOf('"', endPos+1) + 1;
    endPos = data.indexOf('"', startPos);
    // 提取双引号内的内容
    KeyValue = data.substring(startPos, endPos);
    Serial.println(KeyValue);
    
}

void control(void)
{

//键名:前进
  if(KeyName.equals("btn-frt"))
  {//键值判断
    if(KeyValue.equals("press")) //长按
    {
      motor_forward();
    }
    else if(KeyValue.equals("pressup"))//长按松开
    {
      motor_stop();    
    }
  }
//键名:左转
  if(KeyName.equals("btn-left"))
  {//键值判断
    if(KeyValue.equals("press")) //长按
    {
      motor_left();
    }
    else if(KeyValue.equals("pressup"))//长按松开
    {
      motor_stop();         
    }
  }
//键名:右转
  if(KeyName.equals("btn-right"))
  {//键值判断
    if(KeyValue.equals("press")) //长按
    {
      motor_right();
    }
    else if(KeyValue.equals("pressup"))//长按松开
    {
      motor_stop();        
    }
  }
//键名:后退
  if(KeyName.equals("btn-back"))
  {//键值判断
    if(KeyValue.equals("press")) //长按
    {
      motor_back();
    }
    else if(KeyValue.equals("pressup"))//长按松开
    {
      motor_stop();        
    }
  }
//键名:停车
  if(KeyName.equals("btn-stop"))
  {//键值判断
    if(KeyValue.equals("tap")) //轻触
    {
      motor_stop();
    }
    else if(KeyValue.equals("press")) //长按
    {
      motor_stop();
    }
    else if(KeyValue.equals("pressup"))//长按松开
    {
      motor_stop();
    }
  }
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

