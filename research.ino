#include <Servo.h>
Servo base;
Servo shoulder;
Servo elbow;

void setup() {

Serial.begin(9600);
long int timer = millis();

}

// TODO: Add the ports for servos 
void loop() {
  float x, y, z;
  float L1 = 10;
  float L2 = 21;
  Angles joint;
  Coordinates end_effector;
  if(Serial.available() == 0){
    if(millis() - timer > 1000){
      Serial.println("Waiting for a command");
      timer = millis();
    
    }
    return;
  }  

  char input[32];
  int len = Serial.readBytesUntil('\n', input, sizeof(input) - 1);
  input[len] = '\0';
  char* x_str = strtok(input, ",");
  char* y_str = strtok(NULL, ",");
  char* z_str = strtok(NULL, ",");

  x = atof(x_str);
  y = atof(y_str);
  z = atof(z_str);
  joint = InverseKinematics(x,y,z,L1,L2);
  base.write(joint.alpha_deg);
  shoulder.write(joint.shoulder_deg);
  elbow.write(joint.elbow_deg);
  theta0 = base.read();
  theta1 = shoulder.read();
  theta2 = elbow.read();
  end_effector = ForwardKinematics(theta0,theta1,theta2,L1,L2);
    if (Serial.read() == "DATA"){
      DataCollection(x, end_effector.x, y, end_effector.y, z, end_effector.z);
    }
}


struct Coordinates{
  float x;
  float y;
  float z;
};

struct Angles{
  float alpha_deg;
  float shoulder_deg;
  float elbow_deg;
};

struct PID_data{
  float overall;
  float last_error;
  float last_time;
}
void PID_movement(){
  float x, y, z;
  float L1 = 10;
  float L2 = 21;
  PID_data pid;
  Angles joint;
  Coordinates end_effector;
  if(Serial.available() == 0){
    if(millis() - timer > 1000){
      Serial.println("Waiting for a command");
      timer = millis();
    
    }
    return;
  }  

  char input[32];
  int len = Serial.readBytesUntil('\n', input, sizeof(input) - 1);
  input[len] = '\0';
  char* x_str = strtok(input, ",");
  char* y_str = strtok(NULL, ",");
  char* z_str = strtok(NULL, ",");
  x = atof(x_str);
  y = atof(y_str);
  z = atof(z_str);
  joint = InverseKinematics(x,y,z,L1,L2);
  base.write(joint.alpha_deg);
  shoulder.write(joint.shoulder_deg);
  elbow.write(joint.elbow_deg);
  theta0 = base.read();
  theta1 = shoulder.read();
  theta2 = elbow.read();
  end_effector = ForwardKinematics(theta0,theta1,theta2,L1,L2);
  float xt = end_effector.x;
  float yt = end_effector.y;
  float zt = end_effector.z;
  float error = sqrt(pow(xt - x,2) + pow(yt - y,2) + pow(zt - z,2));
  float last_error = 0;
  float last_time = millis();
  pid = PID(error,last_error,millis());
  float pid_processed_base, pid_processed_elbow, pid_processed_shoulder;
  float error_base = base.read() - joint.alpha_degree;
  float error_shoulder = shoulder.read() - joint.shoulder_degree;
  float error_elbow = elbow.read() - joint.elbow_degree;
  float last_error_base = 0, last_error_shoulder = 0, last_error_elbow = 0;
  PID_data pid_processed_base, pid_processed_shoulder, pid_processed_elbow;
  
    while(error >= THRESHOLD){  

      error_base = base.read() - joint.alpha_degree;
      error_shoulder = shoulder.read() - joint.shoulder_degree;
      error_elbow = elbow.read() - joint.elbow_degree;

      pid_processed_base = PID(error_base,last_error_base,last_time); 
      pid_processed_shoulder = PID(error_shoulder,last_error_shoulder,last_time);
      pid_processed_elbow = PID(error_elbow,last_error_elbow,last_time);

      base.write(pid_processed_base.overall);
      shoulder.write(pid_processed_shoulder.overall);
      elbow.write(pid_processed_elbow.overall);

      theta0 = base.read();
      theta1 = shoulder.read();
      theta2 = elbow.read();

      end_effector = ForwardKinematics(theta0,theta1,theta2,L1,L2);

      xt = end_effector.x;
      yt = end_effector.y;
      zt = end_effector.z;

      last_error_base = pid_processed_base.last_error;
      last_error_shoulder = pid_processed_shoulder.last_error;
      last_error_elbow = pid_processed_elbow.last_error;

      last_time = pid_processed_base.last_time;

      error = sqrt(pow(xt - x,2) + pow(yt - y,2) + pow(zt - z,2));
      
}


float PID(float error, float last_error, float last_time){
  PID_data pid;
  float MAX,MIN;
  float kp = 1;
  float ki = 1;
  float kd = 1;
  float proportional = kp * error;
  float integral = ki * (millis() - last_time) * error;
  if(integral >= MAX){
    integral = MAX;
  }
  if(integral <= MIN){
    integral = MIN;
  }
  float derivative = kd * (error - last_error) / (millis() - last_time);
  pid.overall = proportional + integral + derivative;
  pid.last_time = millis();
  pid.last_error = error;
  return pid;
}


void DataCollection(float xt,float x,float yt,float y,float zt,float z){
  float error = sqrt(pow(xt - x,2) + pow(yt - y,2) + pow(zt - z,2));
  Serial.print("Overall x-coordinate: ");
  Serial.println(x);
  Serial.print("Overall y-coordinate: ");
  Serial.println(y);
  Serial.print("Overall z-coordinate: ");
  Serial.println(z);
  Serial.print("Estimated error:");
  Serial.println(error);
}


Coordinates ForwardKinematics(float theta0, float theta1, float theta2, float L1, float L2){
  pi = 3.1415;
  Coordinates coordinates;
  theta0 = theta0 * pi / 180;
  theta1 = theta1 * pi / 180;
  theta2 = theta2 * pi / 180;
  float theta_total = theta1 + theta2;
  float r = (L1 * cos(theta1)) + (L2 * cos(theta_total));
  coordinates.z = (L1 * sin(theta1)) + (L2 * sin(theta_total));
  coordinates.x = r * cos(theta0);
  coordinates.y = r * sin(theta0);
  return coordinates;
}


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
