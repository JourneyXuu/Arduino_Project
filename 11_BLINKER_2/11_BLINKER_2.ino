#define BLINKER_WIFI

#include <Blinker.h>

char auth[] = "e6fab95e8ac0";
char ssid[] = "ikun";
char pswd[] = "2nianban";

// 新建组件对象
BlinkerButton Button1("btn-led");
BlinkerNumber Number1("num-abc");

int counter = 0;
String KeyName;
String KeyValue;

#define LED_BUILTIN 48

void control(void);

// 按下按键即会执行该函数
void button1_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}


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

void setup() {
    // 初始化串口
    Serial.begin(115200);

    BLINKER_DEBUG.stream(Serial);
    
    // 初始化有LED的IO
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(48, LOW);  
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Button1.attach(button1_callback);
}

void loop() {
    Blinker.run();
    control();
}


void control(void)
{
  //键名:点灯
  if(KeyName.equals("btn-led"))
  {//键值判断
    if(KeyValue.equals("tap")) //长按
    {
      digitalWrite(48, !digitalRead(48));
    }
  }
//键名:前进
  if(KeyName.equals("btn-frt"))
  {//键值判断
    if(KeyValue.equals("press")) //长按
    {
      digitalWrite(48, HIGH);
    }
    else if(KeyValue.equals("pressup"))//长按松开
    {
      digitalWrite(48, LOW);     
    }
  }
//键名:左转
  if(KeyName.equals("btn-left"))
  {//键值判断
    if(KeyValue.equals("press")) //长按
    {
      digitalWrite(48, HIGH);
    }
    else if(KeyValue.equals("pressup"))//长按松开
    {
      digitalWrite(48, LOW);          
    }
  }
//键名:右转
  if(KeyName.equals("btn-right"))
  {//键值判断
    if(KeyValue.equals("press")) //长按
    {
      digitalWrite(48, HIGH);
    }
    else if(KeyValue.equals("pressup"))//长按松开
    {
      digitalWrite(48, LOW);         
    }
  }
//键名:右转
  if(KeyName.equals("btn-right"))
  {//键值判断
    if(KeyValue.equals("press")) //长按
    {
      digitalWrite(48, HIGH);
    }
    else if(KeyValue.equals("pressup"))//长按松开
    {
      digitalWrite(48, LOW);          
    }
  }
//键名:后退
  if(KeyName.equals("btn-back"))
  {//键值判断
    if(KeyValue.equals("press")) //长按
    {
      digitalWrite(48, HIGH);
    }
    else if(KeyValue.equals("pressup"))//长按松开
    {
      digitalWrite(48, LOW);          
    }
  }
//键名:后退
  if(KeyName.equals("btn-stop"))
  {//键值判断
    if(KeyValue.equals("tap")) //轻触
    {
      digitalWrite(48, HIGH);
    }
    else if(KeyValue.equals("press")) //长按
    {
      
    }
    else if(KeyValue.equals("pressup"))//长按松开
    {
      
    }

  }


}
