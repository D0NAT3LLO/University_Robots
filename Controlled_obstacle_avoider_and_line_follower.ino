
#define REMOTEXY_MODE__SOFTSERIAL
#include <SoftwareSerial.h>
#include <Servo.h>
#include <NewPing.h>
#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL_RX 2 //grey rx 6 while moov tx 2
#define REMOTEXY_SERIAL_TX 6
#define REMOTEXY_SERIAL_SPEED 9600

int IR1=5;      //Right sensor
int IR2=4;    //left Sensor
// motor one
int enA = 3;    //Right motor
int MotorAip1=13;
int MotorAip2=12;
// motor two
int enB = 11;    //Left motor
int MotorBip1=8;
int MotorBip2=7;
////
int buzzer = 9; //buzzer to arduino pin 9
int trigPin = A2;
int echoPin = A1;
long cm;
int maximumcm = 100;
int MotorSpeed;

// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,6,0,0,0,118,0,13,13,1,
  1,0,25,27,12,12,2,31,70,0,
  1,0,11,40,12,12,2,31,76,0,
  1,0,39,40,12,12,2,31,82,0,
  1,0,25,53,12,12,2,31,66,0,
  2,0,23,5,19,9,2,26,31,31,
  79,78,0,79,70,70,0,4,0,27,
  71,7,18,2,26,129,0,12,92,38,
  4,16,109,111,116,111,114,32,115,112,
  101,101,100,32,99,111,110,116,114,111,
  108,0,129,0,17,16,31,4,16,70,
  114,101,101,32,100,114,105,118,101,32,
  109,111,100,101,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t forward; // =1 if button pressed, else =0 
  uint8_t left; // =1 if button pressed, else =0 
  uint8_t right; // =1 if button pressed, else =0 
  uint8_t back; // =1 if button pressed, else =0 
  uint8_t Switch; // =1 if switch ON and =0 if OFF 
  int8_t SliderValue; // =0..100 slider position 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)


NewPing sonar(trigPin, echoPin, maximumcm);
//Servo servo_motor;

void setup() 
{
  RemoteXY_Init (); 
  Serial.begin(9600);
    // put your setup code here, to run once:
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(IR1,INPUT);
  pinMode(IR2,INPUT);
  pinMode(MotorAip1,OUTPUT);
  pinMode(MotorAip2,OUTPUT);
  pinMode(MotorBip1,OUTPUT);
  pinMode(MotorBip2,OUTPUT);
  //pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output

//
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
 // servo_motor.attach(10); //our servo pin
  // servo_motor.write(40);
  
  delay(2000);
  cm = readPing();
  delay(100);
  cm = readPing();
  delay(100);
  cm = readPing();
  delay(100);
  cm = readPing();
  delay(100);
  
  Serial.begin(9600);
  
}

void loop() 
{ 
 RemoteXY_Handler ();

 if (RemoteXY.Switch) {
        
        if (RemoteXY.forward)
        {
        Forward();
        MotorSpeed = RemoteXY.SliderValue*2.55;
         }
          
      if (RemoteXY.back)
        {
        Backward();
        MotorSpeed = RemoteXY.SliderValue*2.55;
         }
          
      if (RemoteXY.left)
        {
        Left();
        MotorSpeed = RemoteXY.SliderValue*2.55;
         }
          
      if (RemoteXY.right)
        {
        Right();
        MotorSpeed = RemoteXY.SliderValue*2.55;
         }
  
       if (RemoteXY.right == 0 && RemoteXY.left==0  && RemoteXY.forward==0 && RemoteXY.back==0) {
          MotorSpeed = RemoteXY.SliderValue*2.55;
          Stop();
        }
        
   } else {
    
  cm = readPing();
  MotorSpeed=70; // initial if we exit the switch on loop we have this not the last slider value
   if(digitalRead(IR1)==HIGH && digitalRead(IR2)==HIGH) //IR will not glow on black line
  {
    //Stop both Motors
  Stop();
  }

  else if(digitalRead(IR1)==LOW && digitalRead(IR2)==LOW)  //IR not on black line
  {
//    Move both the Motors
    CheckObstacle();
  Forward();
  
  }

  else if(digitalRead(IR1)==LOW && digitalRead(IR2)==HIGH)
  {
    //Tilt robot towards left by stopping the left wheel and moving the right one
    CheckObstacle();
    Left();
    delay(50);
  }

  else if(digitalRead(IR1)==HIGH && digitalRead(IR2)==LOW)
  {
    //Tilt robot towards right by stopping the right wheel and moving the left one
    CheckObstacle();
   Right();
   delay(50);
  }

  else
  {
    //Stop both the motors
 Stop();
  }
  //cm = readPing();
    } 
  }

//FUNCTIONS 


int readPing(){
  //delay(100);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
  //delay(100);
}

void Stop(){
    digitalWrite(MotorAip1,LOW);
    digitalWrite(MotorAip2,LOW);
    digitalWrite(MotorBip1,LOW);
    digitalWrite(MotorBip2,LOW);
   analogWrite (enA, 0);
   analogWrite (enB, 0);
   
}
void Forward(){
   //Move both the Motors
    digitalWrite(MotorAip1,HIGH);
    digitalWrite(MotorAip2,LOW);
    digitalWrite(MotorBip1,HIGH);
    digitalWrite(MotorBip2,LOW);
    analogWrite (enA, MotorSpeed);
    analogWrite (enB, MotorSpeed);
}
void Backward(){
   //Move both the Motors
    digitalWrite(MotorAip1,LOW);
    digitalWrite(MotorAip2,HIGH);
    digitalWrite(MotorBip1,LOW);
    digitalWrite(MotorBip2,HIGH);
    analogWrite (enA, MotorSpeed);
    analogWrite (enB, MotorSpeed);
}
void Left(){ //Tilt robot towards left by stopping the left wheel and moving the right one
    digitalWrite(MotorAip1,LOW);
    digitalWrite(MotorAip2,LOW);
    digitalWrite(MotorBip1,HIGH);
    digitalWrite(MotorBip2,LOW);
    analogWrite (enA, 0);
    analogWrite (enB, MotorSpeed);
   //delay(100);
}
void Right(){
   digitalWrite(MotorAip1,HIGH);
    digitalWrite(MotorAip2,LOW);
    digitalWrite(MotorBip1,LOW);
    digitalWrite(MotorBip2,LOW);
    analogWrite (enA, MotorSpeed);
   analogWrite (enB, 0);
   //delay(100);
  }
  
void CheckObstacle()
{
 if (cm < 24){
    tone(buzzer, 1); // Send 1KHz sound signal...
    Stop();
     delay(300);///kanert 300
    noTone(buzzer);     // Stop sound...
     Left();
     delay(500);//kanet 550
     Forward();
     delay(525);//kanet 600
     Right();
     delay(925); //kanet 650 
   Left();
   delay(300);
     }
}
