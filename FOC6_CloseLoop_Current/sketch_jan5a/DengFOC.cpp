
#include <Arduino.h> 
#include <AS5600.h>
#include "lowpass_filter.h"
#include "pid.h"
#include "InlineCurrent.h"   //引入在线电流检测

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
#define _1_SQRT3 0.57735026919f
#define _2_SQRT3 1.15470053838f

/* Function:限幅函数_constrain，使得传入amt限制在low，high*/
#define _constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

/************private class Init************/

//低通滤波初始化
LowPassFilter M0_Vel_Flt = LowPassFilter(0.01); // Tf = 10ms   //M0速度环
LowPassFilter M0_Curr_Flt = LowPassFilter(0.05); // Tf = 5ms   //M0电流环
//PID
PIDController vel_loop_M0(2, 0, 0, 100000, voltage_power_supply/2);
PIDController angle_loop_M0(2, 0, 0, 100000, 100);
PIDController current_loop_M0 = PIDController{1.2,0,0,100000,12.6};       //P I D ramp limit
//AS5600
Sensor_AS5600 S0=Sensor_AS5600(0);
TwoWire S0_I2C = TwoWire(0);
CurrSense CS_M0= CurrSense(0);

/************private Function************/

//=================PID 设置函数=================
//速度PID
void DFOC_M0_SET_VEL_PID(float P,float I,float D,float ramp)   //M0角度环PID设置
{
  vel_loop_M0.P=P;
  vel_loop_M0.I=I;
  vel_loop_M0.D=D;
  vel_loop_M0.output_ramp=ramp;
}
//角度PID
void DFOC_M0_SET_ANGLE_PID(float P,float I,float D,float ramp)   //M0角度环PID设置
{
  angle_loop_M0.P=P;
  angle_loop_M0.I=I;
  angle_loop_M0.D=D;
  angle_loop_M0.output_ramp=ramp;
}
//电流PID
void DFOC_M0_SET_CURRENT_PID(float P,float I,float D,float ramp)    //M0电流环PID设置
{
  current_loop_M0.P=P;
  current_loop_M0.I=I;
  current_loop_M0.D=D;
  current_loop_M0.output_ramp=ramp;
}

//M0速度PID接口
float DFOC_M0_VEL_PID(float error)   //M0速度环
{
   return vel_loop_M0(error);
   
}
//M0角度PID接口
float DFOC_M0_ANGLE_PID(float error)
{
  return angle_loop_M0(error);
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

void setTorque(float Uq,float angle_el) {
  Uq=_constrain(Uq,-(voltage_power_supply)/2,(voltage_power_supply)/2);
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

  //AS5600
  S0_I2C.begin(4,16, 400000UL);
  S0.Sensor_init(&S0_I2C);   //初始化编码器0
  Serial.println("编码器加载完毕");

  //PID 加载
  // vel_loop_M0 = PIDController{.P = 2, .I = 0, .D = 0, .ramp = 100000, .limit = voltage_power_supply/2};
   vel_loop_M0 = PIDController{2,0,0,100000,voltage_power_supply/2};
 }


float _electricalAngle(){
  return  _normalizeAngle((float)(DIR *  PP) * S0.getMechanicalAngle()-zero_electric_angle);
}


void DFOC_alignSensor(int _PP,int _DIR)
{ 
  PP=_PP;
  DIR=_DIR;
  setTorque(3, _3PI_2);  //起劲
  delay(1000);
  S0.Sensor_update();  //更新角度，方便下面电角度读取
  zero_electric_angle=_electricalAngle();
  setTorque(0, _3PI_2);  //松劲（解除校准）
  Serial.print("0电角度：");Serial.println(zero_electric_angle);
}

float DFOC_M0_Angle()
{
  return DIR*S0.getAngle();
}


//=========================电流读取=========================

//通过Ia,Ib,Ic计算Iq,Id(目前仅输出Iq)
float cal_Iq_Id(float current_a,float current_b,float angle_el)
{
  float I_alpha=current_a;
  float I_beta = _1_SQRT3 * current_a + _2_SQRT3 * current_b;

  float ct = cos(angle_el);
  float st = sin(angle_el);
  //float I_d = I_alpha * ct + I_beta * st;
  float I_q = I_beta * ct - I_alpha * st;
  return I_q;
}

float DFOC_M0_Current()
{  
  float I_q_M0_ori=cal_Iq_Id(CS_M0.current_a,CS_M0.current_b,_electricalAngle());
  float I_q_M0_flit=M0_Curr_Flt(I_q_M0_ori);
  return I_q_M0_flit;  
}

//=========================电流读取=========================
//有滤波
float DFOC_M0_Velocity()
{
  //获取速度数据并滤波
  float vel_M0_ori=S0.getVelocity();
  float vel_M0_flit=M0_Vel_Flt(DIR*vel_M0_ori);
  return vel_M0_flit;   //考虑方向
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


float serial_motor_target()
{
  return motor_target;
}



//================简易接口函数================
void DFOC_M0_set_Velocity_Angle(float Target)
{
 setTorque(DFOC_M0_VEL_PID(DFOC_M0_ANGLE_PID((Target-DFOC_M0_Angle())*180/PI)),_electricalAngle());   //角度闭环
}

void DFOC_M0_setVelocity(float Target)
{
  setTorque(DFOC_M0_VEL_PID((serial_motor_target()-DFOC_M0_Velocity())*180/PI),_electricalAngle());   //速度闭环
}

void DFOC_M0_set_Force_Angle(float Target)   //力位
{
  setTorque(DFOC_M0_ANGLE_PID((Target-DFOC_M0_Angle())*180/PI),_electricalAngle());
}

void DFOC_M0_setTorque(float Target)
{
  setTorque(Target,_electricalAngle());
}


void runFOC()
{
  //====传感器更新====
  S0.Sensor_update();
  CS_M0.getPhaseCurrents();
  
  //====传感器更新====
}

