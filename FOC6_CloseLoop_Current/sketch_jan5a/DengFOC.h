

// struct MOTOR{
// float Ualpha;
// float Ubeta;
// float Ua;
// float Ub;
// float Uc;
// }

/*接口: 1.角度闭环 2.速度闭环 3.力位 4.设置力矩 */
void DFOC_M0_set_Velocity_Angle(float Target);
void DFOC_M0_setVelocity(float Target);
void DFOC_M0_set_Force_Angle(float Target);
void DFOC_M0_setTorque(float Target);

void DFOC_Vbus(float power_supply);
void DFOC_alignSensor(int _PP,int _DIR);

void setTorque(float Uq, float angle_el);
void setPwm(float Ua, float Ub, float Uc);
float _normalizeAngle(float angle);
float _electricalAngle();
 
//传感器读取
float DFOC_M0_Velocity();
float DFOC_M0_Angle();
float DFOC_M0_Current();

//PID
void DFOC_M0_SET_ANGLE_PID(float P,float I,float D,float ramp);
void DFOC_M0_SET_VEL_PID(float P,float I,float D,float ramp);
void DFOC_M0_SET_CURRENT_PID(float P,float I,float D,float ramp);
float DFOC_M0_VEL_PID(float error);
float DFOC_M0_ANGLE_PID(float error);


String serialReceiveUserCommand();
float serial_motor_target();

//runFOC 循环函数
void runFOC();