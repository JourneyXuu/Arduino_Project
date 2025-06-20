/**
Deng's FOC 闭环速度控制例程 测试库：SimpleFOC 2.1.1 测试硬件：KUNFOC控制器
在串口窗口中输入：T+速度，就可以使得两个电机闭环转动
比如让两个电机都以 10rad/s 的速度转动，则输入：T10
在使用自己的电机时，请一定记得修改默认极对数，即 BLDCMotor(7) 中的值，设置为自己的极对数数字
程序默认设置的供电电压为 7.4V,用其他电压供电请记得修改 voltage_power_supply , voltage_limit 变量中的值
默认PID针对的电机是 Ipower 4008 ，使用自己的电机需要修改PID参数，才能实现更好效果
 */
#include <SimpleFOC.h>

MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);

TwoWire I2Cone = TwoWire(0);


//电机参数
BLDCMotor motor = BLDCMotor(11);
BLDCDriver3PWM driver = BLDCDriver3PWM(17, 5, 18, 19);



//命令设置
float target_velocity = 20;
Commander command = Commander(Serial);
void doTarget(char* cmd) { command.scalar(&target_velocity, cmd); }

void setup() {
  I2Cone.begin(4, 16, 400000); 

  sensor.init(&I2Cone);

  //连接motor对象与传感器对象
  motor.linkSensor(&sensor);


  //供电电压设置 [V]
  driver.voltage_power_supply = 12;
  driver.init();


  //连接电机和driver对象
  motor.linkDriver(&driver);
 
  //FOC模型选择
  motor.foc_modulation = FOCModulationType::SpaceVectorPWM;

  //运动控制模式设置
  motor.controller = MotionControlType::velocity;
 


  //速度PI环设置
  motor.PID_velocity.P = 0.2;

  motor.PID_velocity.I = 20;

  //最大电机限制电机
  motor.voltage_limit = 5;

  
  //速度低通滤波时间常数
  motor.LPF_velocity.Tf = 0.01;


  //设置最大速度限制
  motor.velocity_limit = 40;


  Serial.begin(115200);
  motor.useMonitoring(Serial);

  
  //初始化电机
  motor.init();

  //初始化 FOC
  motor.initFOC();

  command.add('T', doTarget, "target velocity");

  Serial.println(F("Motor ready."));
  Serial.println(F("Set the target velocity using serial terminal:"));
  
}



void loop() {
  motor.loopFOC();


  motor.move(target_velocity);


  command.run();
}
