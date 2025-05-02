
#include "AS5600.h"


//PWM输出引脚定义
//IO 宏
#define m1_pwma 17
#define m1_pwmb 5
#define m1_pwmc 18
#define m1_en 19

#define m2_pwma 17
#define m2_pwmb 5
#define m2_pwmc 18
#define m2_en 25

//初始变量及函数定义
#define _constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
//宏定义实现的一个约束函数,用于限制一个值的范围。
//具体来说，该宏定义的名称为 _constrain，接受三个参数 amt、low 和 high，分别表示要限制的值、最小值和最大值。该宏定义的实现使用了三元运算符，根据 amt 是否小于 low 或大于 high，返回其中的最大或最小值，或者返回原值。
//换句话说，如果 amt 小于 low，则返回 low；如果 amt 大于 high，则返回 high；否则返回 amt。这样，_constrain(amt, low, high) 就会将 amt 约束在 [low, high] 的范围内。
float voltage_limit = 10;
float voltage_power_supply = 12;
float shaft_angle=0,open_loop_timestamp=0;
float zero_electric_angle=0,Ualpha,Ubeta=0,Ua=0,Ub=0,Uc=0,dc_a=0,dc_b=0,dc_c=0;
#define _3PI_2 4.71238898038f

int PP=7,DIR=-1;

// 电角度求解
float _electricalAngle(){
  return  _normalizeAngle((float)(DIR *  PP) * getAngle_Without_track()-zero_electric_angle);
}

// 归一化角度到 [0,2PI]
float _normalizeAngle(float angle){
  float a = fmod(angle, 2*PI);   //取余运算可以用于归一化，列出特殊值例子算便知
  return a >= 0 ? a : (a + 2*PI);  
  //三目运算符。格式：condition ? expr1 : expr2 
  //其中，condition 是要求值的条件表达式，如果条件成立，则返回 expr1 的值，否则返回 expr2 的值。可以将三目运算符视为 if-else 语句的简化形式。
  //fmod 函数的余数的符号与除数相同。因此，当 angle 的值为负数时，余数的符号将与 _2PI 的符号相反。也就是说，如果 angle 的值小于 0 且 _2PI 的值为正数，则 fmod(angle, _2PI) 的余数将为负数。
  //例如，当 angle 的值为 -PI/2，_2PI 的值为 2PI 时，fmod(angle, _2PI) 将返回一个负数。在这种情况下，可以通过将负数的余数加上 _2PI 来将角度归一化到 [0, 2PI] 的范围内，以确保角度的值始终为正数。
}


// 设置PWM到控制器输出
void setPwm(float Ua, float Ub, float Uc) {

  Ua = _constrain(Ua,0.0f,voltage_limit);
  Ub = _constrain(Ub,0.0f,voltage_limit);  
  Uc = _constrain(Uc,0.0f,voltage_limit);

  // 计算占空比
  // 限制占空比从0到1
  dc_a = _constrain(Ua / voltage_power_supply, 0.0f , 1.0f );
  dc_b = _constrain(Ub / voltage_power_supply, 0.0f , 1.0f );
  dc_c = _constrain(Uc / voltage_power_supply, 0.0f , 1.0f );

  //写入PWM到PWM 0 1 2 通道
  digitalWrite(m1_en,HIGH);
  ledcWrite(m1_pwma, dc_a*255);
  ledcWrite(m1_pwmb, dc_b*255);
  ledcWrite(m1_pwmc, dc_c*255);
}
/*Fuction:传入Uq、Ud
 *经过帕克逆变换和克拉肯逆变换
 *得到三相电压Ua,Ub,Uc
*/
void setPhaseVoltage(float Uq,float Ud, float angle_el) {
  angle_el = _normalizeAngle(angle_el + zero_electric_angle);
  // 帕克逆变换
  Ualpha =  -Uq*sin(angle_el); 
  Ubeta =   Uq*cos(angle_el); 

  // 克拉克逆变换
  Ua = Ualpha + voltage_power_supply/2;
  Ub = (sqrt(3)*Ubeta-Ualpha)/2 + voltage_power_supply/2;
  Uc = (-Ualpha-sqrt(3)*Ubeta)/2 + voltage_power_supply/2;
  setPwm(Ua,Ub,Uc);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //PWM设置
  pinMode(m1_pwma, OUTPUT);
  pinMode(m1_pwmb, OUTPUT);
  pinMode(m1_pwmc, OUTPUT);
  pinMode(m1_en,OUTPUT);

  ledcAttach(m1_pwma,40000,8);
  ledcAttach(m1_pwmb,40000,8);
  ledcAttach(m1_pwmc,40000,8);

  Serial.println("完成PWM初始化设置");

  BeginSensor();        //I2C通讯初始化
  setPhaseVoltage(3, 0,_3PI_2);

  delay(3000);

  zero_electric_angle=_electricalAngle();

  setPhaseVoltage(0, 0,_3PI_2);
  Serial.print("0电角度：");Serial.println(zero_electric_angle);
}


//==============串口接收==============
float motor_target;
int commaPosition;
String serialReceiveUserCommand() {
  
  // a string to hold incoming data
  static String received_chars;
  
  String command = "";

  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the string buffer:
    received_chars += inChar;

    // end of user input
    if (inChar == '\n') {
      
      // execute the user command
      command = received_chars;

      commaPosition = command.indexOf('\n');//检测字符串中的逗号
      if(commaPosition != -1)//如果有逗号存在就向下执行
      {
          motor_target = command.substring(0,commaPosition).toDouble();            //电机角度
          Serial.println(motor_target);
      }
      // reset the command buffer 
      received_chars = "";
    }
  }
  return command;
}



void loop() {
  // put your main code here, to run repeatedly:
Serial.println(String(getAngle()) + "," + String(motor_target));
  float Sensor_Angle=getAngle();
  float Kp=0.133;
  setPhaseVoltage(_constrain(   Kp*(motor_target-DIR*Sensor_Angle)*180/PI,-6,6  ),0,_electricalAngle());
  serialReceiveUserCommand();

}