#define POT       1

// 初始化模拟输入值
int adc_value;
 
void setup() {
  //初始化串口0，波特率为9600
  Serial.begin(9600);
  //初始化ADC引脚
  pinMode(POT,INPUT);
  // 设置 ADC 分辨率
  analogReadResolution(12);
  // 配置衰减器
  analogSetAttenuation(ADC_11db);
}

void loop() {
  // 获取模拟输入值
  adc_value= analogRead(POT);
  
  // 串口0打印ADC值
  Serial.write("ADC_value = ");
  Serial.println(adc_value);
 
  // 串口0打印 实际电压值
  Serial.write("voltage(V) = ");
  Serial.println(adc_value/4095.0*3.3);
  delay(1000);
}
