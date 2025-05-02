//宏定义
#define Freq 2000
#define Resolution 8 //分辨率
#define LED 48

void setup() {
  // put your setup code here, to run once:
  ledcAttach(LED,Freq,Resolution);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0;i< pow(2,Resolution) ; i++)
{
  ledcWrite(LED,i);
  delay(5);
}
  for (int i = pow(2,Resolution);i>0 ; i--)
{
  ledcWrite(LED,i);
  delay(5);
}

}
