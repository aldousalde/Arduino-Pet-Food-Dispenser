#include <MFRC522.h>
String voice;
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   
// Create MFRC522 instance.
// Include the Servo library 
#include <Servo.h> 
// Declare the Servo pin 
int servoPin = 3; 

int distanceSensor=A0;
// Create a servo object 
Servo Servo1; 

int redPin=5;  
int greenPin=7; 
int bluetoothPin=4;

#include <Time.h>
#include <DS1307RTC.h>

#include <Wire.h>
#include <SparkFun_APDS9960.h>

#define APDS9960_INT    2  // Needs to be an interrupt pin
#define LED_PIN         6 
#define LIGHT_INT_HIGH  1023// High light level for interrupt
#define LIGHT_INT_LOW   255   // Low light level for interrupt

SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint16_t ambient_light = 0;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;
int isr_flag = 0;
uint16_t threshold = 0;
int readValue;  // Use this variable to read Potentiometer
int writeValue;
int delayon=1000;

void setup() {
  pinMode(redPin, OUTPUT); //Set redPin to be an output
pinMode(greenPin, OUTPUT); //Set greenPin to be an output
pinMode(bluetoothPin,OUTPUT);

  pinMode(LED_PIN, OUTPUT);
  pinMode(APDS9960_INT, INPUT);
  
  // Initialize Serial port
  Serial.begin(9600);
  setSyncProvider(RTC.get);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  // We need to attach the servo to the used pin number 
   Servo1.attach(servoPin); 
  Serial.println();
  Serial.println(F("-------------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - Light Interrupts"));
  Serial.println(F("-------------------------------------"));
  
  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);
  
  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  
  // Set high and low interrupt thresholds
  if ( !apds.setLightIntLowThreshold(LIGHT_INT_LOW) ) {
    Serial.println(F("Error writing low threshold"));
  }
  if ( !apds.setLightIntHighThreshold(LIGHT_INT_HIGH) ) {
    Serial.println(F("Error writing high threshold"));
  }
  
  // Start running the APDS-9960 light sensor (no interrupts)
  if ( apds.enableLightSensor(false) ) {
    Serial.println(F("Light sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during light sensor init!"));
  }
  
  // Read high and low interrupt thresholds
  if ( !apds.getLightIntLowThreshold(threshold) ) {
    Serial.println(F("Error reading low threshold"));
  } else {
    Serial.print(F("Low Threshold: "));
    Serial.println(threshold);
  }
  if ( !apds.getLightIntHighThreshold(threshold) ) {
    Serial.println(F("Error reading high threshold"));
  } else {
    Serial.print(F("High Threshold: "));
    Serial.println(threshold);
  }
  
  // Enable interrupts
  if ( !apds.setAmbientLightIntEnable(1) ) {
    Serial.println(F("Error enabling interrupts"));
  }
  
  // Wait for initialization and calibration to finish
  delay(500);
}

void loop() {
  while (Serial.available())
 {
 delay(10);
 char c = Serial.read();
voice = c;
  digitalWrite(bluetoothPin, HIGH);
  delay(250);
  digitalWrite(bluetoothPin, LOW);
  delay(50);
  digitalWrite(bluetoothPin, HIGH);
  delay(250);
  digitalWrite(bluetoothPin, LOW);
  delay(50);
  digitalWrite(bluetoothPin, HIGH);
  delay(250);
  digitalWrite(bluetoothPin, LOW);
  delay(50);
               
    // Make servo go to 0 degrees 
   Servo1.write(0); 
   delay(delayon);
   Servo1.write(180); 
   delay(1000); 
 
 }

// If interrupt occurs, print out the light levels
  if ( isr_flag == 1 ) {
int valueFromDistanceSensor = analogRead(distanceSensor);
 int distance= 4800/(valueFromDistanceSensor - 20);
 if (distance<=6){         
  analogWrite(greenPin,255); 
    delay(250);
    analogWrite(greenPin,0); 
    delay(50);
     analogWrite(greenPin,255); 
    delay(250);
    analogWrite(greenPin,0);
    delay(50);  
     analogWrite(greenPin,255); 
    delay(250);
    analogWrite(greenPin,0); 
    delay(50);        

    // Make servo go to 0 degrees 
    Servo1.write(0); 
   delay(delayon);
   Servo1.write(180); 
   delay(1000); 
   
 }
   
     Serial.print("Time=");
    Serial.print(__TIME__);
    Serial.print(", Date=");
    Serial.println(__DATE__);
  if ((hour(8)) && (minute(0) ) && (second(0))){
                 
                 // Make servo go to 0 degrees 
   Servo1.write(0); 
   delay(delayon);
   Servo1.write(180); 
   delay(1000); 
 }

  if ((hour(12)) && (minute(0) ) && (second(0))){
                 
                 // Make servo go to 0 degrees 
   Servo1.write(0); 
   delay(delayon);
   Servo1.write(180); 
   delay(1000); 
 }

  if ((hour(18)) && (minute(0) ) && (second(0))){
                 
                // Make servo go to 0 degrees 
   Servo1.write(0); 
   delay(delayon);
   Servo1.write(180); 
   delay(1000); 
 }

    // Read the light levels (ambient, red, green, blue) and print
    if (  !apds.readAmbientLight(ambient_light) ||
          !apds.readRedLight(red_light) ||
          !apds.readGreenLight(green_light) ||
          !apds.readBlueLight(blue_light) ) {
      Serial.println("Error reading light values");
    } else {
      Serial.print(" Ambient Light: ");
      Serial.print(ambient_light);
      Serial.print(" | ");
      
    
      if (ambient_light>=2000){
    digitalWrite(LED_PIN, LOW); 
    }
    if (ambient_light<=1999){
    digitalWrite(LED_PIN, HIGH); 
    }
    }
    

    // Reset flag and clear APDS-9960 interrupt (IMPORTANT!)
    isr_flag = 0;
    if ( !apds.clearAmbientLightInt() ) {
      Serial.println("Error clearing interrupt");
    }
    Serial.print("Distance: ");
 Serial.print(distance);
 Serial.print(" | ");
 Serial.print("Dog Food: ");
 if (distance<=5){
 Serial.print("FEED!");
 }

 Serial.println(" ");
 delay(1000);
 
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "73 E9 42 83") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(1000);
   analogWrite(greenPin,255); 
    delay(250);
    analogWrite(greenPin,0); 
    delay(50);
     analogWrite(greenPin,255); 
    delay(250);
    analogWrite(greenPin,0); 
    delay(50);  
     analogWrite(greenPin,255); 
    delay(250);
    analogWrite(greenPin,0); 
    delay(50);  
    
                // Make servo go to 0 degrees 
   Servo1.write(0); 
   delay(delayon);
   Servo1.write(180); 
   delay(1000); 
  }
 
 else   {
    Serial.println(" Access denied");
    delay(3000);
    analogWrite(redPin,255); 
    delay(250);
    analogWrite(redPin,0); 
    delay(100);
     analogWrite(redPin,255); 
    delay(250);
    analogWrite(redPin,0); 
    delay(100);
    analogWrite(redPin,255); 
    delay(250);
    analogWrite(redPin,0); 
    delay(100);
  }
  }
}

void interruptRoutine() {
  isr_flag = 1;
}
