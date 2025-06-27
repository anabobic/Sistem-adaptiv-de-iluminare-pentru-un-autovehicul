
//configurare RemoteXY (BLE mode)
#define REMOTEXY_MODE__ESP32CORE_BLE
#include <BLEDevice.h>
#include <RemoteXY.h>
#define REMOTEXY_BLUETOOTH_NAME "RemoteXY"


#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] = { 
  255,0,0,3,0,87,0,19,0,0,0,0,31,1,106,200,1,1,5,0,
  66,20,47,65,29,131,2,24,70,23,152,18,18,16,26,16,0,70,66,152,
  18,18,16,26,80,0,129,18,140,71,11,64,17,70,97,122,97,32,32,32,
  32,32,32,32,67,101,97,116,97,0,129,13,32,79,12,64,27,73,110,100,
  105,99,97,116,111,114,32,118,105,114,97,106,101,0 };
#pragma pack(pop)

// Structura RemoteXY
struct {
  int8_t  level_01;      
  uint8_t led_01;       
  uint8_t led_02;        
  uint8_t connect_flag;  
} RemoteXY;



#include <ESP32Servo.h>
#include <DHT.h>


// pini și variabile
#define CLK      19
#define DT       18
#define SW       21
#define IN1      26
#define IN2      27

#define SERVO_LED 25
#define HEAD_PIN  32  
#define FOG_PIN   14   
#define LDR_PIN   34

#define DHT_PIN   4
#define DHTTYPE   DHT11

#define THRESHOLD_LIGHT 2000
#define FOG_HUMIDITY    85

Servo headlightServo;
DHT   dht(DHT_PIN, DHTTYPE);

int  lastClkState;
int  encoderCounter    = 0;  // -1,0,+1
int  currentServoAngle = 90;
int  targetServoAngle  = 90;

unsigned long lastDhtCheck = 0;
const unsigned long DHT_INTERVAL = 2000;


void setup() {
  
  RemoteXY_Init();

  Serial.begin(115200);

  //  Encoder 
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT,  INPUT_PULLUP);
  pinMode(SW,  INPUT_PULLUP);

  //Motor L298N 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  //Headlight PWM 
  pinMode(HEAD_PIN, OUTPUT);
  ledcAttach(HEAD_PIN, 5000, 8);

  // Fog LED
  pinMode(FOG_PIN, OUTPUT);
  digitalWrite(FOG_PIN, LOW);

  // Servo faruri
  headlightServo.setPeriodHertz(50);
  headlightServo.attach(SERVO_LED, 500, 2400);
  headlightServo.write(currentServoAngle);

  // LDR
  pinMode(LDR_PIN, INPUT);

  // DHT11 
  dht.begin();

  lastClkState = digitalRead(CLK);
}


float steeringAngle = 90;
float currentAngle = 90;

void loop() {
 
  RemoteXY_Handler();
  
  float viteza = 55;
  float factor = 1.0 - min(viteza / 130.0, 1.0);      
  int dynamicDelay = 10 + (int)(factor * 100); 

  // Citire encoder 
  int clk = digitalRead(CLK), dt = digitalRead(DT);
  if (clk != lastClkState) {
    encoderCounter += (dt == clk) ? +1 : -1;
    encoderCounter = constrain(encoderCounter, -1, 1);
    lastClkState = clk;
  }

  // Comandă motor și unghi servo
  if      (encoderCounter > 0) { digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  targetServoAngle = 120; }
  else if (encoderCounter < 0) { digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); targetServoAngle =  60; }
  else                         { digitalWrite(IN1, LOW);  digitalWrite(IN2, LOW);  targetServoAngle =  90; }

  if (currentServoAngle < targetServoAngle)      headlightServo.write(++currentServoAngle);
  else if (currentServoAngle > targetServoAngle) headlightServo.write(--currentServoAngle);

  if (digitalRead(SW) == LOW) {
    encoderCounter   = 0;
    targetServoAngle = 90;
    delay(300);
  }
       
  // Misca servo treptat 
 // if (abs(currentAngle - steeringAngle) > 1) {
 //  if (currentAngle < steeringAngle) currentAngle++;
 //   else if (currentAngle > steeringAngle) currentAngle--;
 //   headlightServo.write((int)currentAngle);
 //   delay(dynamicDelay);  
 // }


  // Control faruri 
  int LDRValue = analogRead(LDR_PIN);
  bool lowBeam  = (LDRValue >= THRESHOLD_LIGHT);
  bool highBeam = (LDRValue <  THRESHOLD_LIGHT);
  int headDuty  = highBeam ? 255 : 50;
  ledcWrite(HEAD_PIN, headDuty);
  
  if      (!lowBeam && !highBeam) 
  else if ( lowBeam) 
	RemoteXY.led_01 = 0; // scurta
  else                            
	RemoteXY.led_01 = 1; // lunga



if (RemoteXY.led_01 != lastHeadlightState) {
  lastHeadlightState = RemoteXY.led_01;
  switch (RemoteXY.led_01) {
    case 0: Serial.println("Faza scurta activa"); break;
    case 1: Serial.println("Faza lunga activa"); break;
  }


  //Control ceata
  if (millis() - lastDhtCheck >= DHT_INTERVAL) {
    lastDhtCheck = millis();
    float h = dht.readHumidity();
    if (!isnan(h)) {
      bool fog = (h >= FOG_HUMIDITY);
      digitalWrite(FOG_PIN, fog);
      RemoteXY.led_02 = fog;
  
   
    if (RemoteXY.led_02 != lastFogState) {
      lastFogState = RemoteXY.led_02;
 	if (fog) 
	Serial.println("Luminile de ceata activate");
      else     
	Serial.println("Luminile de ceata dezactivate");
	}
  }

  //  Indicator viraje
  RemoteXY.level_01 = map(encoderCounter, -1, 1, 0, 100);


  RemoteXY_delay(20);
}
