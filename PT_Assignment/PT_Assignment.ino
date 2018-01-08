#include <ZumoMotors.h>
#include <Servo.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>

#define LED_PIN 13

// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  1000 // microseconds

// Speed/duration settings

#define REVERSE_SPEED     100
#define TURN_SPEED        150
#define FORWARD_SPEED     100
#define REVERSE_DURATION  250
#define TURN_DURATION     200

Pushbutton button(ZUMO_BUTTON);
ZumoBuzzer buzzer;
ZumoReflectanceSensorArray reflectanceSensors(QTR_NO_EMITTER_PIN);
ZumoMotors motors;

#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
bool line_detection();
void setup()
{
    Serial.println("Press button for Callibration!");
    button.waitForButton();
    sensor_callibration();
    // Init LED pin to enable it to be turned on later
    pinMode(LED_PIN, OUTPUT);
    
    // Set up serial monitor for inital testing purposes
    Serial.begin(9600);
  //  Serial.println("waiting...");
  
    Serial.println("Put zumo at the start, and press button to begin!!");
    button.waitForButton();
    start_robot();
}

void loop()
{
    reflectanceSensors.read(sensor_values);
  if (line_detection){
    manual_control();
    }
    else{
      start_robot();
      }
 
}
 bool line_detection(){
      reflectanceSensors.read(sensor_values);
      if (sensor_values[0] > QTR_THRESHOLD)
      {
        // if leftmost sensor detects line, reverse and turn to the right
        motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
        delay(REVERSE_DURATION);
        motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
        delay(TURN_DURATION);
        motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
        return true;
      }
      else if (sensor_values[5] > QTR_THRESHOLD)
      {
        // if rightmost sensor detects line, reverse and turn to the left
      motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
        delay(REVERSE_DURATION);
        motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
        delay(TURN_DURATION);
        motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
           return true;
      }
          return false;
      }         
  void manual_control()
  {
      motors.setLeftSpeed(0); motors.setRightSpeed(0);

    char inputChar;
      Serial.println("Press Complete when turning has finished!");
      while (!inputChar == "C"){
                    inputChar = (char) Serial.read();

        switch (inputChar)
        {
           case 'w': case 'W': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(FORWARD_SPEED); motors.setRightSpeed(FORWARD_SPEED); delay(2); break;
           case 'a': case 'A': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(0); motors.setRightSpeed(FORWARD_SPEED); delay(2); break;
           case 's': case 'S': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(-REVERSE_SPEED); motors.setRightSpeed(-REVERSE_SPEED); delay(2); break;
           case 'd': case 'D': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(FORWARD_SPEED); motors.setRightSpeed(0); delay(2); break;
           case 'q': case 'Q': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(0); motors.setRightSpeed(0); delay(2); break;
        }
    }
  }
        
  void start_robot(){
    
        motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
         if (line_detection)
 {
  Serial.println("Corner found, please navigate manually");
  manual_control();
 }  
  }
  void sensor_callibration(){
   // Initialise the reflectance sensors module
    reflectanceSensors.init();
 
  delay(500);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);        // turn on LED to indicate we are in calibration mode

 unsigned long startTime = millis();
  while(millis() - startTime < 5000)   // make the calibration take 5 seconds
  {
        //turn left
        motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
        delay(2500);
        //turn right
        motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
        delay(2500);
        reflectanceSensors.calibrate();
  }
  motors.setLeftSpeed(0); motors.setRightSpeed(0); delay(2);
  digitalWrite(13, LOW);         // turn off LED to indicate we are through with calibration 
 }
 
