// Deng's FOC 开环速度控制例程 测试库：SimpleFOC 2.1.1 测试硬件：KUNFOC控制器
// 串口中输入"T+数字"设定两个电机的转速，如设置电机以 10rad/s 转动，输入 "T10"，电机上电时默认会以 5rad/s 转动
// 在使用自己的电机时，请一定记得修改默认极对数，即 BLDCMotor(14) 中的值，设置为自己的极对数数字
// 程序默认设置的供电电压为 16.8V,用其他电压供电请记得修改 voltage_power_supply , voltage_limit 变量中的值

#include <SimpleFOC.h>

BLDCMotor motor = BLDCMotor(7);
BLDCDriver3PWM driver = BLDCDriver3PWM(17, 5, 18, 19);

// BLDC motor & driver instance
BLDCMotor motor1 = BLDCMotor(7);
BLDCDriver3PWM driver1  = BLDCDriver3PWM(27, 26, 14, 25);

//目标变量
float target_velocity = 20;

//串口指令设置
Commander command = Commander(Serial);
void doTarget(char* cmd) {
  command.scalar(&target_velocity, cmd);
}

void setup() {

  pinMode(21, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(2, OUTPUT);


  driver.voltage_power_supply = 12;
  driver.init();
  motor.linkDriver(&driver);
  motor.voltage_limit = 5;   // [V]
  motor.velocity_limit = 50; // [rad/s]

  driver1.voltage_power_supply = 12;
  driver1.init();
  motor1.linkDriver(&driver1);
  motor1.voltage_limit = 5;   // [V]
  motor1.velocity_limit = 50; // [rad/s]


  //开环控制模式设置
  motor.controller = MotionControlType::velocity_openloop;
  motor1.controller = MotionControlType::velocity_openloop;

  //初始化硬件
  motor.init();
  motor1.init();


  //增加 T 指令
  command.add('T', doTarget, "target velocity");

  Serial.begin(115200);
  Serial.println("Motor ready!");
  Serial.println("Set target velocity [rad/s]");
  delay(1000);
}

void loop() {
  if (digitalRead(21) == 0)
  {
    target_velocity = target_velocity + 5 ;
    digitalWrite(2,HIGH) ;
    delay(500);
    digitalWrite(2,LOW) ;
  }
  if (digitalRead(15) == 0)

  {
    target_velocity = target_velocity - 5;
    digitalWrite(2,HIGH) ;
    delay(500);
    digitalWrite(2,LOW) ;
  }
  motor.move(target_velocity);
  motor1.move(target_velocity);

  //用户通讯
  command.run();
}
