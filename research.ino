#include <Servo.h>
Servo base;
Servo shoulder;
Servo elbow;
void setup() {

Serial.begin(9600);

long int timer = millis();
}
// TODO: Add the ports for servos and data collection
void loop() {
  float x, y, z;
  float L1 = 10;
  float L2 = 21;
  Angles joints;
while (Serial.read == NULL){
  if(millis() - timer > 1000){
    Serial.println("Waiting for a command");
    timer = millis();
  }
}  
while(Serial.available() > 0){
input = Serial.readStringUntil('\n')

int firstComma = input.indexOf(',')
int secondComma = input.indexOf(',', firstComma + 1)

String x_str = input.substring(0, firstComma)
String y_str = input.substring(firstComma + 1, secondComma)
String z_str = input.substring(secondComma + 1)

x = x_str.toFloat()
y = y_str.toFloat()
z = z_str.toFloat()

}
joint = InverseKinematics(x,y,z,L1,L2);
base.write(joint.alpha_deg);
shoulder.write(joint.shoulder_deg);
elbow.write(joint.elbow_deg);
}
struct Angles{
  float alpha_deg;
  float shoulder_deg;
  float elbow_deg;
};
Angles InverseKinematics(float x, float y, float z, float L1, float L2){ 
  Angles result;
  float pi = 3.1415;
  float alpha = atan2(y,x);
  float l = sqrt(x*x + y*y);
  float h = sqrt(l*l + z*z);
  float D = (h*h - L1*L1 - L2*L2)/(2*L1*L2);
  if (D > 1){
    D = 1;
    }
  else if (D < -1){
    D = -1;
    }
  float theta_elbow = acos(D);
  float beta  = atan2(z,l);
  float gamma_numerator = L2 * sin(theta_elbow);
  float gamma_denominator = L1 + L2 * cos(theta_elbow);
  float gamma = atan2(gamma_numerator,gamma_denominator);
  float theta_shoulder = beta - gamma;
  result.alpha_deg = alpha * 180 / pi;
  result.shoulder_deg = theta_shoulder * 180 / pi;
  result.elbow_deg = theta_elbow * 180 / pi;

  return result;
}
