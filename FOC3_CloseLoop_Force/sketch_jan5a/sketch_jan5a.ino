//灯哥开源，遵循GNU协议，转载请著名版权！
//GNU开源协议（GNU General Public License, GPL）是一种自由软件许可协议，保障用户能够自由地使用、研究、分享和修改软件。
//该协议的主要特点是，要求任何修改或衍生的作品必须以相同的方式公开发布，即必须开源。此外，该协议也要求在使用或分发软件时，必须保留版权信息和许可协议。GNU开源协议是自由软件基金会（FSF）制定和维护的一种协议，常用于GNU计划的软件和其他自由软件中。

#include "DengFOC.h"

int Sensor_DIR=-1;    //传感器方向 顺时针为正，逆时针为负
int Motor_PP=7;    //电机极对数

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //PWM设置
  DFOC_Vbus(12);        //驱动器电压12V
  DFOC_alignSensor(Motor_PP,Sensor_DIR);
}
         

void loop() {
  //输出角度值
  //Serial.print("当前角度：");
  //Serial.println(DFOC_M0_Angle());

  //输出角度值
  float Kp=0.133;
  float Sensor_Angle=DFOC_M0_Angle();
  setTorque(Kp*(serial_motor_target()-Sensor_DIR*Sensor_Angle)*180/PI,_electricalAngle());   //位置闭环
  // setTorque(serial_motor_target(),_electricalAngle());   //电压力矩
  serialReceiveUserCommand();

}