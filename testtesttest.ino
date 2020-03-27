#define enA 9              // define statements for pin numbers
#define in1 6              //
#define in2 7              //
#define readA bitRead(2,2) // faster then digital read
#define readB bitRead(3,3) //
#define enc1 2             //
#define enc2 3             //
#define button 4           //
int rotDirection=0;        //
int pressed=false;         //
volatile double count=0;   //
double period=0;           //
double freq1=0;            //
double rpm1=0;             //
double lastmillis=0;       // for rpm calculations
void encoder_calc();       // function to calculate rpm of encoder
double sampletime=250;        // in micros for encoder sample time
double pos = 0;            // position of shaft
void pwm();                //
double gear_ratio=1440; 
double deg1=0;//

void setup() // setup
{
  
  Serial.begin(9600);                                           // initialize serial monitor at 9600 baud rate
  pinMode(enc1,INPUT_PULLUP);                                   // Sets pin 2 to INPUT
  pinMode(enc2,INPUT_PULLUP);                                   //Sets pin 3 (interupt to input pullup)
  pinMode(enA,OUTPUT);                                          //Sets pin as enable
  pinMode(in1,OUTPUT);                                          // sets in1 as an OUTPUT
  pinMode(in2,OUTPUT);                                          //
  pinMode(button,INPUT);                                        //
  digitalWrite(in1,LOW);                                        //
  digitalWrite(in2,HIGH);                                       // sets the initial direction of rotation of the motor (H bridge)
  attachInterrupt(digitalPinToInterrupt(enc1),encoder1,CHANGE); // attach pins 2 and optical encoder arduino interupts3 to interupts with function encoder 1 and encoder 2 respectively
  attachInterrupt(digitalPinToInterrupt(enc2),encoder2,CHANGE); //
  lastmillis=millis();                                          // Sets your last micros value as micros this should be the last thing done in setup

}

void loop() // main
{
  int potValue = analogRead(0);                    // Read potentiometer value
  int pwmOutput = map(potValue, 0, 1023, 0 , 255); // Map the potentiometer value from 0 to 255
  analogWrite(enA, pwmOutput);                     // Send PWM signal to L298N Enable pin
  pwm();                                           //
  Serial.println("Degrees");
}



void pwm() 
{
 
  encoder_calc(); // call the encoder calc function (it will also serial print)
  
  // Read button - Debounce
  if (digitalRead(button) == true) 
  {
    pressed = !pressed;
  }
  while (digitalRead(button) == true); // not quite sure what this accomplishes
  delay(20);
  
  // If button is pressed - change rotation direction
  if (pressed == true  & rotDirection == 0) 
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    rotDirection = 1;
    delay(20);
  }
  
  // If button is pressed - change rotation direction
  if (pressed == false & rotDirection == 1) 
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    rotDirection = 0;
    delay(20);
  }
}

void encoder_calc()
{
 if(millis()-lastmillis>sampletime)
  {
   detachInterrupt(digitalPinToInterrupt(enc1)); 
   detachInterrupt(digitalPinToInterrupt(enc2));
   period=millis()-lastmillis;
   freq1=count;
   rpm1 = ((1.395*freq1)/(period));
   deg1=rpm1*.006*period;
   
   if (rotDirection == 1)
   {
    pos = pos + deg1;
   }
   else
   {
    pos = pos - deg1;
   }
   
   count=0;
   Serial.println("Degrees");
   Serial.print(pos,5);
   lastmillis=millis();
   attachInterrupt(digitalPinToInterrupt(enc1),encoder1,CHANGE); 
   
   // attach pins 2 and optical encoder arduino interupts3 to interupts with function encoder 1 and encoder 2 respectively
   
   attachInterrupt(digitalPinToInterrupt(enc2),encoder2,CHANGE);
   
  }
}
  
void encoder1()
{
  if(readB!=readA)
  {
    count--;
  } 
  else
  {
    count++;
  }
}

void encoder2()
{
  if(readA==readB)
  {
    count--;
  } 
  else
  {
    count++;
  }
}
