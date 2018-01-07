#include <ZumoMotors.h>
#include <Servo.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>

#define LED_PIN 13

// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  500 // microseconds

// Speed/duration settings
#define REVERSE_SPEED     200
#define TURN_SPEED        200
#define FORWARD_SPEED     200
#define REVERSE_DURATION  200
#define TURN_DURATION     300

ZumoBuzzer buzzer;
ZumoReflectanceSensorArray reflectanceSensors(QTR_NO_EMITTER_PIN);
ZumoMotors motors;

#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];

void setup()
{
    // Initialise the reflectance sensors module
    reflectanceSensors.init();

    // Init LED pin to enable it to be turned on later
    pinMode(LED_PIN, OUTPUT);

    // Set up serial monitor for inital testing purposes
    Serial.begin(9600);
    Serial.println("waiting...");
}

void loop()
{
  if (Serial.available()>0)
  {
    char inputChar = (char) Serial.read();
    if (inputChar == "W" || "w")
    {
      reflectanceSensors.read(sensor_values);
      if (sensor_values[0] > QTR_THRESHOLD)
      {
        // if leftmost sensor detects line, reverse and turn to the right
        motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
        delay(REVERSE_DURATION);
        motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
        delay(TURN_DURATION);
        motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
      }
      else if (sensor_values[5] > QTR_THRESHOLD)
      {
        // if rightmost sensor detects line, reverse and turn to the left
        motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
        delay(REVERSE_DURATION);
        motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
        delay(TURN_DURATION);
        motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
      }
      else
      {
        // otherwise, go straight
        motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
      }      
    }
  }
  
  /*if (Serial.available()>0) {
        char inputChar = (char) Serial.read();
        switch (inputChar){
           case 'w': case 'W': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(FORWARD_SPEED); motors.setRightSpeed(FORWARD_SPEED); delay(2); break;
           case 'a': case 'A': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(0); motors.setRightSpeed(FORWARD_SPEED); delay(2); break;
           case 's': case 'S': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(-REVERSE_SPEED); motors.setRightSpeed(-REVERSE_SPEED); delay(2); break;
           case 'd': case 'D': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(FORWARD_SPEED); motors.setRightSpeed(0); delay(2); break;
           case 'q': case 'Q': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(0); motors.setRightSpeed(0); delay(2); break;
        }
  }*/
}
