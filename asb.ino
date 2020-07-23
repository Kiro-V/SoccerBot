

#include <AFMotor.h>
#include <SoftwareSerial.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

// Initialize variables
int movement;
int byte_number;
int action;
int pwm;
int angle;
int rgb;

unsigned long previousMillis = 0;        // will store last time LED was updated
long OnTime = 500;           // milliseconds of on-time

#define FADESPEED 5
#define in1 A12
#define in2 A13
#define ENB  9
#define relayPin  A10
#define REDPIN 44
#define GREENPIN 45
#define BLUEPIN 46
#define myServo 10
 
SoftwareSerial mybluetooth(A8, A9);

// FORWARD
void forward() { 
  motor3.run(FORWARD);
  motor2.run(FORWARD);
  motor4.run(FORWARD);
  motor1.run(FORWARD);
}

// BACKWARD
void backward() { 
  motor3.run(BACKWARD);
  motor2.run(BACKWARD);
  motor4.run(BACKWARD);
  motor1.run(BACKWARD);
}

// RIGHT
void right() {
  motor3.run(FORWARD);
  motor2.run(BACKWARD);
  motor4.run(BACKWARD);
  motor1.run(FORWARD);
}

// LEFT
void left() {
  motor3.run(BACKWARD);
  motor2.run(FORWARD);
  motor4.run(FORWARD);
  motor1.run(BACKWARD);
}

// RIGHT FORWARD
void right_forward() { 
  motor3.run(FORWARD);
  motor2.run(RELEASE);
  motor4.run(RELEASE);
  motor1.run(FORWARD);
}

// LEFT FORWARD
void left_forward() { 
  motor3.run(RELEASE);
  motor2.run(FORWARD);
  motor4.run(FORWARD);
  motor1.run(RELEASE);
}

// RIGHT BACKWARD
void right_backward() {
  motor3.run(RELEASE);
  motor2.run(BACKWARD);
  motor4.run(BACKWARD);
  motor1.run(RELEASE);
}

//LEFT BACKWARD
void left_backward() {  
  motor3.run(BACKWARD);
  motor2.run(RELEASE);
  motor4.run(RELEASE);
  motor1.run(BACKWARD);
}

// RIGHT ROTATE
void right_rotate() {
  motor3.run(FORWARD);
  motor2.run(BACKWARD);
  motor4.run(FORWARD);
  motor1.run(BACKWARD);
}

// LEFT ROTATE
void left_rotate() {
  motor3.run(BACKWARD);
  motor2.run(FORWARD);
  motor4.run(BACKWARD);
  motor1.run(FORWARD);
}

// STOP
void stop_moving() {
  motor3.run(RELEASE);
  motor2.run(RELEASE);
  motor4.run(RELEASE);
  motor1.run(RELEASE);
}
// OFF DRIBBLER
void off() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(ENB, 0);
}
void servowrite ( int angle){
  auto runUntil = millis() +500;
  while (millis() < runUntil) {
    pwm = (angle*11) + 500;      // Convert angle to microseconds
    digitalWrite(myServo, HIGH);
    delayMicroseconds(pwm);
    digitalWrite(myServo, LOW);
    delay(50);   // Refresh cycle of servo
    yield();
  }
}

// LED ON
void led_on() {
  int r, g, b;
  // fade from blue to violet
  for (r = 0; r < 256; r++) { 
    analogWrite(REDPIN, r);
    delay(FADESPEED);
  } 
  // fade from violet to red
  for (b = 255; b > 0; b--) { 
    analogWrite(BLUEPIN, b);
    delay(FADESPEED);
  } 
  // fade from red to yellow
  for (g = 0; g < 256; g++) { 
    analogWrite(GREENPIN, g);
    delay(FADESPEED);
  } 
  // fade from yellow to green
  for (r = 255; r > 0; r--) { 
    analogWrite(REDPIN, r);
    delay(FADESPEED);
  } 
  // fade from green to teal
  for (b = 0; b < 256; b++) { 
    analogWrite(BLUEPIN, b);
    delay(FADESPEED);
  } 
  // fade from teal to blue
  for (g = 255; g > 0; g--) { 
    analogWrite(GREENPIN, g);
    delay(FADESPEED);
  } 
}

void led_off() {
  analogWrite(REDPIN, 0);
  analogWrite(GREENPIN, 0);
  analogWrite(BLUEPIN, 0);
}
void setup() {
  Serial.begin(115200);
  
  pinMode( myServo, OUTPUT);
  
  mybluetooth.begin(9600);
// Set speed motor
  motor1.setSpeed(225);
  motor2.setSpeed(200);
  motor3.setSpeed(200);
  motor4.setSpeed(200);
  
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

// Setup L298N
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ENB, OUTPUT);
  analogWrite(ENB, 0);

// LED
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);  

}

void loop() { 

  // Read incoming data 
    while (mybluetooth.available() > 0)
    {
      byte_number = mybluetooth.read();
      if (byte_number >= 48 & byte_number <= 58)
      {
        movement = byte_number;
      
      // Movements
        if (movement == '2') {
          forward();
        }
        else if (movement == '7') {
          backward();
        }
        else if (movement == '5') {
          right();
        }
        else if (movement == '4') {
          left();
        }
        else if (movement == '3') {
          right_forward(); 
        }
        else if (movement == '1') {
          left_forward();
        }
        else if (movement == '8') {
          right_backward();
        }
        else if (movement == '6') {
          left_backward();
        }
        else if (movement == '9') {
          left_rotate();
        }
        else if (movement == ':') {
          right_rotate();
        }
        else if (movement == '0') {
          stop_moving();
        }

      }
      else
      {
        action = byte_number;
      // Actions
        if (action == 'f') {
          digitalWrite(relayPin, HIGH);
          delay(100);
          digitalWrite(relayPin ,LOW);
        }
    
      // Servo Control
        if (action == 'L') {   
          servowrite(5);
        }
        if (action == 'M' || (action == 'f')) {
          servowrite(55);
        }   
        if (action == 'R') {
          servowrite(95);

        }
      
      // Dribbler Control
        if (action == 'p') {
          off();
        }
        if (action == 'o') {
          analogWrite(ENB, 40);
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
        }
        if (action == 'c') {
          analogWrite(ENB, 40);
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
        }
        if (action == 'b') {
          analogWrite(ENB, 70);
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
        }
        if (action == 'x') {
            led_on();
        }
        else if (action == 'y') {
              led_off();
            }

        
      
    }
}
}
