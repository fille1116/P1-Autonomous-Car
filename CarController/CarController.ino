/* 
* CarController  
*  
* By Philip E, Henrik F 
* 2017-04-05 12:05 
*  
* Program for stopping motor in the event of obstacle 
* and allowing steering 
*/ 
 
#include <Servo.h> 
 
Servo pointer; 
const byte SERVO_PIN = 8;  
 
/* Motor Driver */ 
const byte DIR1_PINA = 5;  //Backwards pin 
const byte DIR2_PINA = 3; //Forwards pin 
const byte SPEED_PINA = 11; // Needs to be a PWM pin to be able to control motor speed 
const byte MOTOR_SPEED = 200; //Change motor speed 
//50 
const byte MIDDLE_POINT_SERVO = 46; 
int command = 0; 
int lastCommand = 0; 
int time = 50; 

void setup(){ 
	Serial.begin(115200);  
	 
	//Define L298N Dual H-Bridge Motor Controller Pins 

	pinMode(DIR1_PINA,OUTPUT); 
	pinMode(DIR2_PINA,OUTPUT); 
	pinMode(SPEED_PINA,OUTPUT); 
	pointer.attach(SERVO_PIN); 

	motor_run(0, 1, 10); 
	steer(MIDDLE_POINT_SERVO, 10); 
} 
 
void loop(){ 
    if (Serial.available() > 0){ 
      command = Serial.read(); 
    } 
     
    send_command(command,time); 
    lastCommand = command; 
} 

void reset(){ 
	motor_run(0, 0, 50); 

	steer(MIDDLE_POINT_SERVO, 50);
} 
 
/* 
*   The golden method! Control the motor with 
*   a certain speed (0 to 100). Direction: 0 reverse, 1 forward. 
*/ 

void motor_run(int speed, int direction, int time){ 
    if(direction == 1){ 
        analogWrite(SPEED_PINA, speed);//Sets speed variable via PWM  
        digitalWrite(DIR1_PINA, LOW); 
        digitalWrite(DIR2_PINA, HIGH); 
    } 
    else if (direction == 0) { 
        analogWrite(SPEED_PINA, speed); 
        digitalWrite(DIR1_PINA, HIGH); 
        digitalWrite(DIR2_PINA, LOW); 
    } 
 
    delay(time); 
} 

void steer(int capacity, int time){ 
    if(capacity > 100 || capacity < 0){ 
       capacity = MIDDLE_POINT_SERVO; 
    } 
     
    int adjustment = 10.24 * capacity; 
     
    //Steering between 65 degrees and 105 degrees from car right. 
    //(20 degrees from the normal)
    int pos = map(adjustment, 0, 1023, 55, 105);
    pointer.write(pos); 
 
    delay(time); 
} 

void send_command(int command, int time){ 
  switch (command){ 
 
     //reset command 
     case 0: reset(); break; 
 
     // single command 
     case 1: 
     	motor_run(MOTOR_SPEED, 1, time);  
        break; 
     case 2: 
     	motor_run(MOTOR_SPEED, 0, time); 
        break; 
     case 3: 
     	steer(0, time);  
        break; 
     case 4: 
     	steer(100, time);  
        break; 
 
     //combination command 
     case 6: 
     	motor_run(MOTOR_SPEED, 1, time); 
        steer(0, time); 
        break; 
     case 7: 
     	motor_run(MOTOR_SPEED, 1, time); 
        steer(100,time); 
        break; 
     case 8: 
     	steer(0,time); 
        motor_run(MOTOR_SPEED, 0, time); 
        break; 
     case 9: 
     	steer(100,time); 
        motor_run(MOTOR_SPEED, 0, time); 
        break; 
     default: Serial.print("Invalid Command\n"); 
    } 
} 









