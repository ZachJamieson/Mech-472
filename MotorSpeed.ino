#include <Servo.h>

Servo Motor;
Servo Steering;

int PWM = 1500;
int Read1;
int Read2;

void setup() 
{
  Serial.begin(115200);
  Motor.attach(10);
  Steering.attach(9);
  Motor.writeMicroseconds(PWM);
}

void loop() 
{
  if (Serial.available() > 0) 
  {
    char inChar = Serial.read();
    Serial.println(inChar);
    if ( inChar == '1' )
    {
      PWM++;
    }
    else if ( inChar == '2')
    {
      PWM--;
    }
  }
  Read1 = analogRead(A1);
  Read2 = analogRead(A2);
  Motor.writeMicroseconds(PWM);
  Steering.write(75);
  Serial.print(PWM);
  Serial.print("\t");
  Serial.print(Read1);
  Serial.print("\t");
  Serial.println(Read2);
}
