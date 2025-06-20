//DengFOC V0.2
//灯哥开源，遵循GNU协议，转载请著名版权！
//GNU开源协议（GNU General Public License, GPL）是一种自由软件许可协议，保障用户能够自由地使用、研究、分享和修改软件。
//该协议的主要特点是，要求任何修改或衍生的作品必须以相同的方式公开发布，即必须开源。此外，该协议也要求在使用或分发软件时，必须保留版权信息和许可协议。GNU开源协议是自由软件基金会（FSF）制定和维护的一种协议，常用于GNU计划的软件和其他自由软件中。
//仅在DengFOC官方硬件上测试过，欢迎硬件购买/支持作者，淘宝搜索店铺：灯哥开源
//你的支持将是接下来做视频和持续开源的经费，灯哥在这里先谢谢大家了

#include "DengFOC.h"


void setup() {
  Serial.begin(115200);
  DFOC_Vbus(12);   //设定驱动器供电电压
  DFOC_M0_alignSensor(7,-1);
  DFOC_M1_alignSensor(7,-1);
  //DFOC_M1_alignSensor(7,1);
  DFOC_enable();
  
}

int count=0;
void loop() 
{
  runFOC();
  // DFOC_M0_setTorque(1);
  // DFOC_M1_setTorque(1);

  //力位（加入电流环后）
  // DFOC_M0_SET_ANGLE_PID(0.5,0,0.003,100000,0.1);
  // DFOC_M0_SET_CURRENT_PID(1.25,50,0,100000);
  // DFOC_M0_set_Force_Angle(serial_motor_target());
  // DFOC_M1_SET_ANGLE_PID(0.5,0,0.003,100000,0.1);
  // DFOC_M1_SET_CURRENT_PID(1.25,50,0,100000);
  // DFOC_M1_set_Force_Angle(serial_motor_target());

  //速度（加入电流环后）
  // DFOC_M0_SET_VEL_PID(3,2,0,100000,0.5);
  // DFOC_M0_SET_CURRENT_PID(0.5,50,0,100000);
  // DFOC_M0_setVelocity(serial_motor_target());
  // DFOC_M0_SET_VEL_PID(0.1,2,0,100000,0.5);
  // DFOC_M0_SET_CURRENT_PID(0.5,50,0,100000);
  // DFOC_M0_setVelocity(serial_motor_target());

  //位置-速度-力（加入电流环后）
  DFOC_M0_SET_ANGLE_PID(1,0,0,100000,30);
  DFOC_M0_SET_VEL_PID(0.02,1,0,100000,0.5);
  DFOC_M0_SET_CURRENT_PID(5,200,0,100000);
  DFOC_M0_set_Velocity_Angle(serial_motor_target());

  //位置-速度-力（加入电流环后）
  DFOC_M1_SET_ANGLE_PID(1,0,0,100000,30);
  DFOC_M1_SET_VEL_PID(0.02,1,0,100000,0.5);
  DFOC_M1_SET_CURRENT_PID(5,200,0,100000);
  DFOC_M1_set_Velocity_Angle(serial_motor_target());
  //电流力矩
  // DFOC_M1_SET_CURRENT_PID(5,200,0,100000);
  // DFOC_M0_SET_CURRENT_PID(5,200,0,100000);
  
  // DFOC_M0_setTorque(serial_motor_target());
  // DFOC_M1_setTorque(serial_motor_target());


  
  count++;
  if(count>100)
  {
      count=0;
      //Serial.printf("%f\n", DFOC_M0_Current());
      Serial.printf("%f,%f,%f,%f\n", DFOC_M0_Current(),DFOC_M0_Angle(), DFOC_M0_Velocity(),serial_motor_target());
      // Serial.printf("%f,%f,%f\n", DFOC_M0_Angle(), S0_electricalAngle(),S1_electricalAngle());
      // Serial.printf("%f,%f,%f\n", DFOC_M0_Current(), DFOC_M1_Current(),serial_motor_target());
  }
  //接收串口
  serialReceiveUserCommand();

}
