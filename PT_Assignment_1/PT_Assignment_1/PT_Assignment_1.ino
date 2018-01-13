/*
 Name:		PT_Assignment_1.ino
 Created:	1/7/2018 4:53:00 PM
 Author:	Rasesh
*/
#pragma once
#include <LSM303.h>
#include <ZumoMotors.h>
#include <Servo.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>
#include <Wire.h>
#define CALIBRATION_SAMPLES 70  // Number of compass readings to take when calibrating
#define CRB_REG_M_2_5GAUSS 0x60 // CRB_REG_M value for magnetometer +/-2.5 gauss full scale
#define CRA_REG_M_220HZ    0x1C // CRA_REG_M value for magnetometer 220 Hz update rate
#define LED_PIN 13


// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  500 // microseconds
#define ABOVE_LINE(sensor)((sensor) > QTR_THRESHOLD)

// Speed/duration settings
#define SPEED           200 // Maximum motor speed when going straight; variable speed when turning
#define TURN_BASE_SPEED 100 // Base speed when turning (added to variable speed)
// Allowed deviation (in degrees) relative to target angle that must be achieved before driving straight
#define DEVIATION_THRESHOLD 5
#define REVERSE_SPEED     100
#define TURN_SPEED        150
#define FORWARD_SPEED     100
#define REVERSE_DURATION  250
#define TURN_DURATION     200
Pushbutton button(ZUMO_BUTTON);
ZumoBuzzer buzzer;
ZumoReflectanceSensorArray reflectanceSensors(QTR_NO_EMITTER_PIN);
ZumoMotors motors;
LSM303 compass;
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
String line_detection();
int zumo_direction;

// Converts x and y components of a vector to a heading in degrees.
// This function is used instead of LSM303::heading() because we don't
// want the acceleration of the Zumo to factor spuriously into the
// tilt compensation that LSM303::heading() performs. This calculation
// assumes that the Zumo is always level.
template <typename T> float heading(LSM303::vector<T> v)
{
	float x_scaled = 2.0*(float)(v.x - compass.m_min.x) / (compass.m_max.x - compass.m_min.x) - 1.0;
	float y_scaled = 2.0*(float)(v.y - compass.m_min.y) / (compass.m_max.y - compass.m_min.y) - 1.0;

	float angle = atan2(y_scaled, x_scaled) * 180 / M_PI;
	if (angle < 0)
		angle += 360;
	return angle;
}
void setup()
{	
	Serial.begin(9600);
	//Initiate the Wire library and join the I2C bus as a master
	Serial.println("Press button for Callibration!");
	//button.waitForButton();
	//initialise_compass();
	/*Serial.println("Place zumo over black line for sensor callibration.");
	button.waitForButton();
	sensor_callibration();*/
	// Init LED pin to enable it to be turned on later
	pinMode(LED_PIN, OUTPUT);

	// Set up serial monitor for inital testing purposes
//	Serial.println("Put zumo at the start, and press button to begin!!");
	//button.waitForButton();
	//auto_navigation();
	//manual_control();
	//outside_room();
//	sensor_callibration();
}


void loop()
{
/*	reflectanceSensors.read(sensor_values);
	Serial.println(sensor_values[0]);
	Serial.println(sensor_values[1]);
	Serial.println(sensor_values[2]);
	Serial.println(sensor_values[3]);
	Serial.println(sensor_values[4]);
	Serial.println(sensor_values[5]);
	delay(5000); // the number of milliseconds between readings   */
	manual_control();


}
String line_detection() {
	reflectanceSensors.read(sensor_values);
	if ((sensor_values[0] > QTR_THRESHOLD) || (sensor_values[5] > QTR_THRESHOLD))
	{
		for (int i = 2;i < 4;i++) { //if any value in the middle sensors are above 300, we are in middle territory
			if (sensor_values[i] > QTR_THRESHOLD) {
				motors.setSpeeds(0, 0);
				motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
				Serial.println(sensor_values[i]);
				Serial.println(i);
				delay(REVERSE_DURATION);
				return "WALL"; //we're facing a wall
			}
		}

		if ((sensor_values[0] > QTR_THRESHOLD)) {

			//	Serial.println("left" + sensor_values[0] + sensor_values[5]);


					// if leftmost sensor detects line, reverse and turn to the right
			motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
			delay(REVERSE_DURATION);
			motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
			delay(TURN_DURATION);
			motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
			//	rotate(fmod(averageHeading() + 180, 360));

			return "LEFT";
		}
		else if ((sensor_values[5] > QTR_THRESHOLD))
		{
			//	Serial.println("right" + sensor_values[5] + sensor_values[0]);

				// if rightmost sensor detects line, reverse and turn to the left
			motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
			delay(REVERSE_DURATION);
			motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
			delay(TURN_DURATION);
			motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
			//rotate(fmod(averageHeading() + 180, 360));

			return "RIGHT";
		}
	}
	else {
		return "N/A";
	}
}


//OPTIMIZE RETURN PATH: 
//STORE THE DELAY FOR THE CORRIDOR 1, NOTE DOWN IF ITS LEFT OR RIGHT, ON RETURN REVERSE THIS

void manual_control()
{
	char inputChar;
	while (inputChar != 'C')
	{

		if (Serial.available() > 0)
		{
			inputChar = Serial.read();
		}
		switch (inputChar)
		{
		case 'w': case 'W': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(FORWARD_SPEED); motors.setRightSpeed(FORWARD_SPEED); delay(REVERSE_DURATION); break;
		case 'a': case 'A': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(-250); motors.setRightSpeed(250); delay(REVERSE_DURATION); break;
		case 's': case 'S': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(-REVERSE_SPEED); motors.setRightSpeed(-REVERSE_SPEED); delay(REVERSE_DURATION); break;
		case 'd': case 'D': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(250); motors.setRightSpeed(-250); delay(REVERSE_DURATION); break;
		case 'r': case 'R': outside_room(); break;
		case 'c': case 'C': corridor(); break;
		}
		inputChar = ' '; //reset
		motors.setLeftSpeed(0); motors.setRightSpeed(0); delay(0);
	}
	/*if (inputChar == 'C') {
	auto_navigation();
	}*/
}

void corridor() {
	Serial.println("Automatic navigation started.");
	char inputChar;
	while (line_detection() != "WALL") {
		//zumo_direction = averageHeading(); //set the direction the zumo is currently facing
		motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
		if (Serial.available() > 0)
		{
			inputChar = Serial.read();
		}
		switch (inputChar)
		{
		case 'stop': case 'STOP': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(0); motors.setRightSpeed(0); delay(REVERSE_DURATION); break;
		case 'r': case 'R': outside_room(); break;
		case 'c': case 'C': corridor(); break;
		inputChar = ' '; //reset
		}

	}
	Serial.println("I've hit a corner! Manual Navigation Initiated. Press 'Complete' when I'm back on route.");
	manual_control();
}

//cURRENT LOCATION!!!!
/*bool borderDetect() {
	reflectanceSensors.read(sensor_values);
	if (sensor_values[0] > QTR_THRESHOLD)
	{
		// if leftmost sensor detects line, reverse and turn to the right
		motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
		delay(REVERSE_DURATION);
		/*motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
		delay(TURN_DURATION);*/
	/*	motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
		rotate(fmod(averageHeading() + 180, 360));

		return true;
	}
	else if (sensor_values[5] > QTR_THRESHOLD)
	{
		// if rightmost sensor detects line, reverse and turn to the left
		motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
		delay(REVERSE_DURATION);
		/*motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
		delay(TURN_DURATION);*/
	/*	motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
		rotate(fmod(averageHeading() + 180, 360));

		return true;
	}
	return false;

}*/
void outside_room() {
	//to do, fix errors and scan from left to right  in order to automatically determine where the door is
	//rotate(zumo_direction);


	//motors.setLeftSpeed(250); motors.setRightSpeed(250); delay(200);
	//motors.setLeftSpeed(-150); motors.setRightSpeed(150); delay(500);
	//motors.setLeftSpeed(250); motors.setRightSpeed(250); delay(200);

	//this will need its only line detector.
		rotate(fmod(averageHeading() + 90, 360));
		while (line_detection() == "N/A") {
			motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
			 //inch forward...
		}

		/*rotate((averageHeading() + 90, 360));
		Serial.println("I've hit a corner! Manual Navigation Initiated. Press 'Complete' when I'm back on route.");	}
	motors.setLeftSpeed(-250); motors.setRightSpeed(-250); delay(200);*/


}


void sensor_callibration() {
	// Initialise the reflectance sensors module
	reflectanceSensors.init();
	delay(500);
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);        // turn on LED to indicate we are in calibration mode

	unsigned long startTime = millis();
	while (millis() - startTime < 5000)   // make the calibration take 5 seconds
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

void initialise_compass() {

	// The highest possible magnetic value to read in any direction is 2047
	// The lowest possible magnetic value to read in any direction is -2047
	LSM303::vector<int16_t> running_min = { 32767, 32767, 32767 }, running_max = { -32767, -32767, -32767 };
	unsigned char index;

	// Initiate the Wire library and join the I2C bus as a master
	Wire.begin();

	// Initiate LSM303
	compass.init();

	// Enables accelerometer and magnetometer
	compass.enableDefault();

	compass.writeReg(LSM303::CRB_REG_M, CRB_REG_M_2_5GAUSS); // +/- 2.5 gauss sensitivity to hopefully avoid overflow problems
	compass.writeReg(LSM303::CRA_REG_M, CRA_REG_M_220HZ);    // 220 Hz compass update rate

															 // To calibrate the magnetometer, the Zumo spins to find the max/min
															 // magnetic vectors. This information is used to correct for offsets
															 // in the magnetometer data.
	motors.setLeftSpeed(SPEED);
	motors.setRightSpeed(-SPEED);

	for (index = 0; index < CALIBRATION_SAMPLES; index++)
	{
		// Take a reading of the magnetic vector and store it in compass.m
		compass.read();

		running_min.x = min(running_min.x, compass.m.x);
		running_min.y = min(running_min.y, compass.m.y);

		running_max.x = max(running_max.x, compass.m.x);
		running_max.y = max(running_max.y, compass.m.y);
		delay(50);
	}

	motors.setLeftSpeed(0);
	motors.setRightSpeed(0);

	// Set calibrated values to compass.m_max and compass.m_min
	compass.m_max.x = running_max.x;
	compass.m_max.y = running_max.y;
	compass.m_min.x = running_min.x;
	compass.m_min.y = running_min.y;
}
// Average 10 vectors to get a better measurement and help smooth out
// the motors' magnetic interference.
float averageHeading()
{
	LSM303::vector<int32_t> avg = { 0, 0, 0 };

	for (int i = 0; i < 10; i++)
	{
		compass.read();
		avg.x += compass.m.x;
		avg.y += compass.m.y;
	}
	avg.x /= 10.0;
	avg.y /= 10.0;

	// avg is the average measure of the magnetic vector.
	return heading(avg);
}

float relativeHeading(float heading_from, float heading_to)
{
	float relative_heading = heading_to - heading_from;

	// constrain to -180 to 180 degree range
	if (relative_heading > 180)
		relative_heading -= 360;
	if (relative_heading < -180)
		relative_heading += 360;

	return relative_heading;
}


void rotate(int degrees) //rotate to an angle based on compass
{
	int speed;
	float heading = averageHeading();


	float relative = relativeHeading(heading, degrees);
	while (abs(relative) > DEVIATION_THRESHOLD)
	{
		heading = averageHeading();
		relative = relativeHeading(heading, degrees);
		speed = SPEED * relative / 180;
		if (speed < 0)
			speed -= TURN_BASE_SPEED;
		else
			speed += TURN_BASE_SPEED;
		motors.setSpeeds(speed, -speed);
	}
	motors.setSpeeds(0, 0);
	delay(500);
}
