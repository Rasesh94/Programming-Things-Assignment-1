#include <NewPing.h>
#include <ZumoBuzzer.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>

//Global Variables
char incomingByte;
char lastByte;

#define QTR_THRESHOLD  1200 // microseconds
#define ABOVE_LINE(sensor)(sensor > QTR_THRESHOLD)
#define REVERSE_SPEED     150 // 0 is stopped, 400 is full speed
#define TURN_SPEED        150
#define FORWARD_SPEED     100
#define REVERSE_DURATION  250 // ms
#define TURN_DURATION     200 // ms

int trig = 2; // attach pin 3 to Trig
int echo = 6; //attach pin 4 to Echo
#define MAX_DISTANCE 500
			  //

ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN); //the sensa
ZumoMotors motors;
ZumoBuzzer buzzer;
NewPing sonar(trig, echo, MAX_DISTANCE);

bool Found = false;
int count = 0;
int roomCount = 0;
int roomsArray[100];
long cm;
char command;
char Way;


#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
bool checkIfWall();
void NavigateCorridoor();

void setup() {
	Serial.begin(9600);

	/* Clear the terminal that we are printing this output to */
	Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

	sonar.ping_cm();
	sensors.init();
}

void loop() {
	// put your main code here, to run repeatedly:
	if (Serial.available() > 0)
	{
		// read the oldest byte in the serial buffer:
		incomingByte = Serial.read();
		lastByte = incomingByte;

	}

	switch (lastByte)
	{
	case 'N':
		//Serial.println("A has been pressed");
		NavigateCorridoor();
		command = ' ';
		break; /* optional */

	case 'r':
		Serial.println("Scanning Room");
		motors.setSpeeds(0, 0);
		roomScan();
		manual();
		lastByte = 'N';
		command = ' ';
		break; /* optional */

	case 'M':
		Serial.println("Manual Mode Activated");
		manual();
		lastByte = ' ';
		command = ' ';
		break; /* optional */
	case 'R':
		motors.setSpeeds(0, 0);
		Serial.println("which way is the room ?");
		Way = ' ';
		while (Way == ' ')
		{
			if (Serial.available() > 0)
			{
				// read the oldest byte in the serial buffer:
				Way = Serial.read();

			}
		}
		if (Way == 'D')
		{
			Serial.println("room is on the right");

		}
		else
		{
			Serial.println("room is on the left");

		}
		command = Way;
		manual();
		lastByte = 'r';
		break;




	case 'X':
		motors.setSpeeds(0, 0);
		Serial.println("robot Stopped");
		lastByte = ' ';
		break;
	}




}


void manual()
{
	while (command != 'C')
	{

		if (Serial.available() > 0)
		{
			// read the oldest byte in the serial buffer:
			command = Serial.read();

		}

		switch (command)
		{
		case 'W':
			motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
			delay(REVERSE_DURATION);
			motors.setSpeeds(0, 0);
			command = ' ';
			break;

		case 'S':
			motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
			delay(REVERSE_DURATION);
			motors.setSpeeds(0, 0);
			command = ' ';
			break;

		case 'A':
			manualTurn("left");
			break;

		case 'D':
			manualTurn("right");
			break;

		case 'C':
			Serial.println("Complete Confirmed");

			break;

		}
	}

}



void NavigateCorridoor()
{
	if (checkIfWall())
	{

		Serial.print("cornerFound");
		manual();
		lastByte = 'N';

	}
	else
	{
		if (sensor_values[0] > QTR_THRESHOLD)
		{
			//Serial.println(sensor_values[0]);
			//buzzer.playNote(NOTE_G(3), 200, 15);
			// if leftmost sensor detects line, reverse and turn to the right
			motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
			delay(REVERSE_DURATION);
			motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
			delay(TURN_DURATION);
			motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
		}
		else if (sensor_values[5] > QTR_THRESHOLD)
		{
			// buzzer.playNote(NOTE_G(2), 200, 15);
			// if rightmost sensor detects line, reverse and turn to the left
			motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
			delay(REVERSE_DURATION);
			motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
			delay(TURN_DURATION);
			motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
		}
	}
}






void manualTurn(String Direction1)
{
	if (Direction1 == "right")
	{
		motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
		delay(TURN_DURATION);
		motors.setSpeeds(0, 0);
		command = ' ';
	}

	if (Direction1 == "left")
	{
		motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
		delay(TURN_DURATION);
		motors.setSpeeds(0, 0);
		command = ' ';
	}



}

bool checkIfWall()
{
	motors.setSpeeds(100, 100);
	sensors.read(sensor_values);
	if (ABOVE_LINE(sensor_values[0]) || ABOVE_LINE(sensor_values[5]))
	{
		delay(50);
		sensors.read(sensor_values);
		delay(5);
		//sensors.read(sensor_values);
		//Serial.println(sensor_values[1]);
		if ((ABOVE_LINE(sensor_values[1])) || (ABOVE_LINE(sensor_values[4])) || (ABOVE_LINE(sensor_values[3])) || (ABOVE_LINE(sensor_values[2])))
		{
			motors.setSpeeds(0, 0);
			motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
			delay(REVERSE_DURATION);
			motors.setSpeeds(0, 0);
			buzzer.playNote(NOTE_G(2), 200, 15);
			//lastByte = 'M';
			//Serial.print("cornerFound");
			return true;

		}
	}
	return false;
}

void roomScan()
{

	Scan("right");
	delay(10);
	Scan("left");
	delay(10);
	Scan("left");
	delay(10);
	Scan("right");
	delay(10);

	motors.setSpeeds(0, 0);
	Found = false;
}

void Scan(String Direction)
{
	if (Found)
	{

	}
	else
	{
		int Motor1Speed, Motor2Speed;
		if (Direction == "right")
		{
			Motor1Speed = 125;
			Motor2Speed = -125;
		}

		if (Direction == "left")
		{
			Motor1Speed = -125;
			Motor2Speed = 125;

		}

		while (count < 7 && !Found)
		{

			cm = 0;
			motors.setSpeeds(Motor1Speed, Motor2Speed);
			delay(150);
			motors.setSpeeds(0, 0);
			delay(20);
			cm = sonar.ping_cm();
			Serial.println(cm);
			if ((cm == 4) || (cm == 5))
			{
				delay(5);
				cm = sonar.ping_cm();
			}

			if ((cm < 10) && (cm > 0))
			{
				Found = true;
				StoreRoom(cm);
				lastByte = 'M';
				break;
			}
			Serial.print(cm);
			Serial.print("cm");
			Serial.print(count);
			Serial.println();
			motors.setSpeeds(Motor1Speed, Motor2Speed);
			count++;
		}
	}

	count = 0;









}
void StoreRoom(long cm)
{

	if (Found)
	{
		roomsArray[roomCount] = 1;
		Serial.print("room ");
		Serial.print(cm);
		Serial.print(":");
		Serial.print(roomsArray[roomCount]);
		Serial.println();
		roomCount++;
		buzzer.play(">g32>>c32");
		delay(200);
		buzzer.play(">g32>>c32");
		delay(200);
		buzzer.play(">g32>>c32");
	}
}