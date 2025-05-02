

// struct MOTOR{
// float Ualpha;
// float Ubeta;
// float Ua;
// float Ub;
// float Uc;
// }

void DFOC_Vbus(float power_supply);
void setPwm(float Ua, float Ub, float Uc);

float _normalizeAngle(float angle);
float _electricalAngle();
 
void setTorque(float Uq, float angle_el);
void DFOC_alignSensor(int _PP,int _DIR);
float DFOC_M0_Angle();

String serialReceiveUserCommand();
float serial_motor_target();


