//IO宏
#define RXPIN 9
#define TXPIN 10

uint8_t DataSent[100];
volatile uint8_t DataReceive[100];

void UART1_ReceiveHandler(void);

//Serial、Serial1、Serial2 分别对应了 UART0、UART1 和 UART2。
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200,SERIAL_8N1,RXPIN,TXPIN);   //数据位为 8 位，N 表示无校验位，1 表示停止位为 1 位
  Serial1.onReceive(UART1_ReceiveHandler);
  Serial1.write("hello world\n",13);
  Serial.println();//换行
}

void loop() {
  // put your main code here, to run repeatedly:


}

void UART1_ReceiveHandler(void)
{
  // volatile uint8_t RxState = 0;
  // volatile uint8_t pRxPacket = 0;

  size_t lenth = Serial1.available();
  while(lenth -- )
  {
  Serial1.print((char)Serial1.read());
  }
  Serial.println();//换行
}
