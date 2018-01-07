#include <ZumoMotors.h>
#include <Servo.h>
/*
 * This example uses the ZumoMotors library to drive each motor on the Zumo
 * forward, then backward. The yellow user LED is on when a motor should be
 * running forward and off when a motor should be running backward. If a
 * motor on your Zumo has been flipped, you can correct its direction by
 * uncommenting the call to flipLeftMotor() or flipRightMotor() in the setup()
 * function.
 */

#define LED_PIN 13
// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  500 // microseconds

// Speed/duration settings
#define REVERSE_SPEED     200
#define TURN_SPEED        200
#define FORWARD_SPEED     200
#define REVERSE_DURATION  200
#define TURN_DURATION     300

ZumoMotors motors;

void setup()
{
  pinMode(LED_PIN, OUTPUT);


    Serial.begin(9600);
    // for (int k = 0; k < 4; k++){
    //     servos[k].attach(pins[k]);
    // }
    Serial.println("waiting...");
  // uncomment one or both of the following lines if your motors' directions need to be flipped
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);
}

void loop()
{

  if (Serial.available()>0) {
        char motor = (char) Serial.read();
        int pin;
        Servo movingServo;
        String name ="";
        switch (motor){
         case 'w': case 'W': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(10000); motors.setRightSpeed(10000); delay(2); break;
         case 'a': case 'A': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(0); motors.setRightSpeed(10000); delay(2); break;
         case 's': case 'S': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(-10000); motors.setRightSpeed(-10000); delay(2); break;
         case 'd': case 'D': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(10000); motors.setRightSpeed(0); delay(2); break;
         case 'q': case 'Q': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(0); motors.setRightSpeed(0); delay(2); break;
        }


      }
}
