#include <Arduino.h> 
#include <AS5600.h>

/************private variable************/

float voltage_limit = 10;
float voltage_power_supply;
float zero_electric_angle=0;
int PP=1,DIR=1;
// struct MOTOR motor = {0};
float Ualpha,Ubeta=0,Ua=0,Ub=0,Uc=0;
/************private define************/
#define m1_pwma 17
#define m1_pwmb 5
#define m1_pwmc 18
#define m1_en 19

#define m2_pwma 17
#define m2_pwmb 5
#define m2_pwmc 18
#define m2_en 25

#define _3PI_2 4.71238898038f 

/* Function:限幅函数_constrain，使得传入amt限制在low，high*/
#define _constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

/************private Function************/



/* Function:DFOC_Vbus 设置电源电压输入和pwm通道
 * Param: power_supply :输入电源供电
 * Note:  
*/
void DFOC_Vbus(float power_supply)
{
  voltage_power_supply=power_supply;
  pinMode(m1_pwma, OUTPUT);
  pinMode(m1_pwmb, OUTPUT);
  pinMode(m1_pwmc, OUTPUT);
  pinMode(m1_en,OUTPUT);

  ledcAttach(m1_pwma,40000,8);  //pwm频道, 频率, 精度
  ledcAttach(m1_pwmb,40000,8);
  ledcAttach(m1_pwmc,40000,8);

  Serial.println("完成PWM初始化设置");
  BeginSensor();
 }


/* Function:设置PWM到控制器输出
 * Param: No
 * Note:  
*/
void setPwm(float Ua, float Ub, float Uc) 
{

  // 计算占空比
  // 限制占空比从0到1
  float dc_a = _constrain(Ua / voltage_power_supply, 0.0f , 1.0f );
  float dc_b = _constrain(Ub / voltage_power_supply, 0.0f , 1.0f );
  float dc_c = _constrain(Uc / voltage_power_supply, 0.0f , 1.0f );

  //写入PWM到PWM 0 1 2 通道
  digitalWrite(m1_en,HIGH);
  ledcWrite(m1_pwma, dc_a*255);
  ledcWrite(m1_pwmb, dc_b*255);
  ledcWrite(m1_pwmc, dc_c*255);
}

/* Function:角度归一化[0,2PI]函数_normalizeAngle
 * Param: angle
 * Note:  
   fmod 函数的余数的符号与除数相同。
   负数的余数加上 _2PI 来将角度归一化到 [0, 2PI] 的范围内，以确保角度的值始终为正数。
*/
float _normalizeAngle(float angle)
{
  float a = fmod(angle, 2*PI);   //取余运算可以用于归一化，列出特殊值例子算便知
  return a >= 0 ? a : (a + 2*PI);  
}

/* Function: _electricalAngle电角度计算
 * Param: 返回归一化后的弧度
 * Note:  

*/
float _electricalAngle()
{
  return  _normalizeAngle((float)(DIR *  PP) * getAngle_Without_track()-zero_electric_angle);
}



/*Fuction:setTorque 设置电机力矩
 *Note:经过帕克逆变换和克拉肯逆变换
 *    得到三相电压Ua,Ub,Uc
*/
void setTorque(float Uq,float angle_el) {
  Uq=_constrain(Uq,-voltage_power_supply/2,voltage_power_supply/2);
  float Ud=0;
  angle_el = _normalizeAngle(angle_el);
  // 帕克逆变换
  Ualpha =  -Uq*sin(angle_el); 
  Ubeta =   Uq*cos(angle_el); 

  // 克拉克逆变换
  Ua = Ualpha + voltage_power_supply/2;
  Ub = (sqrt(3)*Ubeta-Ualpha)/2 + voltage_power_supply/2;
  Uc = (-Ualpha-sqrt(3)*Ubeta)/2 + voltage_power_supply/2;
  setPwm(Ua,Ub,Uc);
}

/*Fuction:DFOC_alignSensor 传感器回零
 *Note:
 *
*/
void DFOC_alignSensor(int _PP,int _DIR)
{ 
  PP=_PP;
  DIR=_DIR;
  setTorque(3, _3PI_2);         //将电机转到270度
  delay(3000);
  zero_electric_angle=_electricalAngle();
  setTorque(0, _3PI_2);          //将电机转到270度
  Serial.print("0电角度：");Serial.println(zero_electric_angle);
}


float DFOC_M0_Angle()
{
  return getAngle();
}

//==============串口接收==============


/*Fuction:serialReceiveUserCommand 串口接收用户指令
 *Note:
 *
*/

float motor_target;
int commaPosition;
String serialReceiveUserCommand() {
  
  // a string to hold incoming data
  static String received_chars;     //静态字符串，用于存储从串口接收到的字符。
  
  String command = "";              //用于存储完整的命令字符串。
  
  while (Serial.available()) {      //接收循环
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

float serial_motor_target()
{
  return motor_target;
}



