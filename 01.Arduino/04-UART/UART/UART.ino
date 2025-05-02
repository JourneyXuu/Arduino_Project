#define RXPIN 9
#define TXPIN 10

uint8_t dataSent[100];

//接收中断回调函数
void onReceiveFunction(void) 
{
  // 接收串口1发送过来的数据长度
  size_t available = Serial1.available();

  //显示接收长度
  Serial.printf("onReceive Callback:: There are %d bytes available: ", available);
  //接收长度不为0则一直减
  while (available --) 
  {
    //通过串口0显示串口1接收到的数据
    Serial.print((char)Serial1.read());
  }
  //显示换行
  Serial.println();
}

//Serial、Serial1、Serial2 分别对应了 UART0、UART1 和 UART2。
void setup() 
{
  // 初始化串口通信波特率
  //串口0初始化
  Serial.begin(115200);

  //初始化串口1，波特率115200，SERIAL_8N1=8数据位无校验位1停止位 RX引脚为9 TX引脚为10
  Serial1.begin(115200, SERIAL_8N1, RXPIN, TXPIN); 
  //为串口1设置回调函数
  Serial1.onReceive(onReceiveFunction); 

  //串口1发送数据 hello LCKFB，发送12个字节
  Serial1.write("hello LCKFB", 12);

}

void loop() 
{
  // 从串口监视器读取输入数据
  if (Serial.available()) 
  {
    char data = Serial.read();

    // 将数据发送到 UART1
    Serial1.write(data);
  }

  // 从UART1读取输入数据
  if (Serial1.available()) 
  {
    char data = Serial1.read();

    // 将数据发送到 UART0
    Serial.write(data);
  }
}
