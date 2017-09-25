// FILE: arduino_sonar_servo
// AUTHOR: grzjur
// VERSION: 0.0.1

#include "RunningMedian.h" //https://playground.arduino.cc/Main/RunningMedian
#include <Servo.h>

#define SERVO_PORT 9
#define TRIG 2
#define ECHO 3

#define SERVO_MIN 0.00
#define SERVO_MAX 180.00
#define HC_SR04_MIN 2.00
#define HC_SR04_MAX 200.00
#define MEDIAN_ARR 7

float m_distance;
float m_time;
float m_pos;
float m_d;
float m_e;
Servo myservo;
RunningMedian median = RunningMedian(MEDIAN_ARR);

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  myservo.attach(SERVO_PORT);
  myservo.write(SERVO_MIN);
  m_d = (SERVO_MAX - SERVO_MIN)/(HC_SR04_MAX-HC_SR04_MIN);
  m_e = SERVO_MIN - (HC_SR04_MIN * m_d);
}

void loop() {
  for (int i=0; i< MEDIAN_ARR; i++){
    distance();
    median.add(m_distance);
    delay(5);
  }
  m_distance = median.getMedian();
  
  if (m_distance<HC_SR04_MIN){m_distance=HC_SR04_MIN;}
  if (m_distance>HC_SR04_MAX){m_distance=HC_SR04_MAX;}
  m_pos = m_distance*m_d + m_e;
  myservo.write(m_pos);

  Serial.print("  Distance: "); Serial.print(m_distance);
  Serial.print(" Servo: "); Serial.println(m_pos);
  
  delay(200);
}

void distance(){
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(15);
  digitalWrite(TRIG, LOW);
  digitalWrite(ECHO, HIGH); 
  m_time = pulseIn(ECHO, HIGH);
  m_distance = m_time / 58.00;
}
