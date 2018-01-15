
/*
 Name:    PT_Assignment_1.ino
 Created: 1/7/2018 4:53:00 PM
 Author:  Rasesh Ramanuj
*/
#include<Vector.h>
#include <LSM303.h>
#include <ZumoMotors.h>
#include <Servo.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>
#include <Wire.h>
#include <NewPing.h>

#define CALIBRATION_SAMPLES 70  // Number of compass readings to take when calibrating
#define CRB_REG_M_2_5GAUSS 0x60 // CRB_REG_M value for magnetometer +/-2.5 gauss full scale
#define CRA_REG_M_220HZ    0x1C // CRA_REG_M value for magnetometer 220 Hz update rate
#define LED_PIN 13
int currentCorridor;
int lastMove, lastRoom;
/*Corridor Class. The purpose of this class is to store data pertaining to each corridor instance the zumo walks upon.
 * Initial confusion arised as to what constitutes to a corridor, i.e. is the whole track one big corridor with the only differences being the 
 * sub corridors? I decided to add a boolean value of 'sub'. So each subcorridor gets it's own class instance, it is just flagged as a sub.
 * originally I attempted to split this out into .h/.cpp files (whilst I was working within Visual Micro) - however this proved to be difficult when I 
 * switched over to Arduino IDE.*/
 //Corridor.h
 
class Corridor {
private:
  int corr_id;
  bool sub;
  int current_direction; // 0 refers to left  , 1 = right
public:
  Corridor();
  int get_corr_id();
  void set_corr_id(int id);
  bool is_sub();
  void set_sub(bool sub_);
  void set_previous_direction(int direction);
  void set_current_direction(int direction);
  int get_current_direction();
};
//Corridor.cpp
Corridor::Corridor() {}
int Corridor::get_corr_id() {
  return corr_id;
}
void Corridor::set_corr_id(int id) {
  corr_id = id;
}
bool Corridor::is_sub() {
  return sub;
}
void Corridor::set_sub(bool sub_) {
  sub = sub_;
}
void Corridor::set_current_direction(int direction) {
  current_direction = direction;
}

int Corridor::get_current_direction() {
  return current_direction;
}

//Room Class. This class stores the room id, objects refers to anything that was found in the room (true/false), corr_id which is 
//corridor the room is in, and the door direction - 1 = left, 0 = right.

//Room.h
class Room {
private:
  int id;
  bool objects;
  int corr_id;
  int door_direction; //1 = left, 0 = right.
public:
  Room();
  //getters
  int get_id();
  bool get_objects();
  int get_corr_id();
  int get_door_direction();
  //setters
  void set_id(int id_);
  void set_objects(bool objects_);
  void set_corr_id(int id_);
  void set_direction(int direction);
};
//Room.cpp
Room::Room() {}
int Room::get_id() {
  return id;
}
int Room::get_door_direction() {
  return door_direction;
}
void Room::set_id(int id_) {
  id = id_;
}
bool Room::get_objects() {
  return objects;
}
void Room::set_objects(bool objects_) {
  objects = objects_;
}
int Room::get_corr_id(){
  return corr_id;
}
void Room::set_corr_id(int id_) {
  corr_id = id_;
}
void Room::set_direction(int direction) {
  door_direction = direction;
}

const int trigPin = 2;
const int echoPin = 6;


//global vectors to store both rooms and corridors. Originally I attempted to create them as locals within the setup function
//and use pointers to manipulate the data, however this proved to be tedious and I didn't have the luxury of time to implement this.
Vector<Room> rooms; //global vectors to 
Vector<Corridor> corridors;

// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  300 // microseconds
//#define ABOVE_LINE(sensor)((sensor) > QTR_THRESHOLD)
// Speed/duration settings
#define SPEED           150 // Maximum motor speed when going straight; variable speed when turning
#define TURN_BASE_SPEED 100 // Base speed when turning (added to variable speed)
// Allowed deviation (in degrees) relative to target angle that must be achieved before driving straight
#define DEVIATION_THRESHOLD 5
#define REVERSE_SPEED     100
#define TURN_SPEED        150
#define FORWARD_SPEED     100
#define MAX_DISTANCE 30
#define REVERSE_DURATION  250
#define TURN_DURATION     200
Pushbutton button(ZUMO_BUTTON);
ZumoBuzzer buzzer;
ZumoReflectanceSensorArray reflectanceSensors(QTR_NO_EMITTER_PIN);
ZumoMotors motors;
LSM303 compass;
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
NewPing sonar(trigPin, echoPin, MAX_DISTANCE);

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
  lastRoom = 0;
  currentCorridor = 0;
  Serial.begin(9600);
  Serial.println("Press button for Callibration!");
  button.waitForButton();
  initialise_compass();
  Serial.println("Place zumo over black line for sensor callibration.");
  button.waitForButton();
  sensor_callibration();
  Serial.println("Put zumo at the start, and press button to begin!!");
  button.waitForButton();
  corridor(false);
}


void loop()
{
}


 //This function gets the left or right from the previous corner/corridor. (Required when barring the user from going in a direction)
int get_previous_move(){
 
  for (int i = 0; i < corridors.size(); ++i)
  {
    if (corridors[i].get_corr_id() == currentCorridor - 1) {
      return corridors[i].get_current_direction();
    }
    }
  }

  //this function returns the current left or right for the corridor
int get_current_move(){
  for (int i = 0; i < corridors.size(); ++i)
  {
    if (corridors[i].get_corr_id() == currentCorridor) {
      return corridors[i].get_current_direction();
    }
    }
  }

//returns whether the corridor is a subcorridor or standard
bool is_corridor_sub(){
  for (int i = 0; i < corridors.size(); ++i)
  {
    if (corridors[i].get_corr_id() == currentCorridor) {
      return corridors[i].is_sub();
    }
    }
  }


//this function is used to differentiate whether the zumo is at a wall, or just going left and right and needs a correction
String line_detection() {
if (check_for_wall()){
  return "WALL";
}
  else
  {
    if (sensor_values[0] > QTR_THRESHOLD)
    {
      // if leftmost sensor detects line, reverse and turn to the right
      motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
      delay(REVERSE_DURATION);
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
      delay(TURN_DURATION);
      motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
      return "L";
    }
    else if (sensor_values[5] > QTR_THRESHOLD)
    {
      // if rightmost sensor detects line, reverse and turn to the left
      motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
      delay(REVERSE_DURATION);
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
      delay(TURN_DURATION);
      motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
      return "R";
    }
  }
  return "N/A";
}

//if sensors other than the far left and right are above the threshold it would indicate we are on a wall. 
  bool check_for_wall() {
  motors.setSpeeds(100, 100);
  reflectanceSensors.read(sensor_values);
  //it's a given one of these values are above the qtr threshold, but we need to check if it's just the left side, or the entire zumo
  if ((sensor_values[0] > QTR_THRESHOLD) || (sensor_values[5]  > QTR_THRESHOLD))
  {
    delay(100); //pause the zumo to read the values in again
    reflectanceSensors.read(sensor_values); 
    if ((sensor_values[1] > QTR_THRESHOLD) || (sensor_values[2] > QTR_THRESHOLD) || (sensor_values[3] > QTR_THRESHOLD) || (sensor_values[4] > QTR_THRESHOLD))
    { //if any of the middle sensors are above the QTR, stop the zumo. 
      motors.setSpeeds(0, 0);
      motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
      delay(REVERSE_DURATION);
      motors.setSpeeds(0, 0);
      return true;
    }
  }
  return false;
}

//Manual Controls
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
        case 'a': case 'A': rotate(fmod(averageHeading() - 93, 360));lastMove = 1;break; //aprox left
        case 's': case 'S': digitalWrite(LED_PIN, HIGH); motors.setLeftSpeed(-REVERSE_SPEED); motors.setRightSpeed(-REVERSE_SPEED); delay(REVERSE_DURATION); break;
        case 'd': case 'D': rotate(fmod(averageHeading() + 93, 360));lastMove = 0;break; //approx right
        case 'c': case 'C': complete();break; //approx right
        case 'r': case 'R': outside_room(); break;
        case 'z': case 'Z': corridor(true); break;
        case 'x': case 'X': motors.setSpeeds(0,0); break;
        case 'l': case 'L': status_report();; break;  //reports current rooms/corridors entered

      }
      motors.setSpeeds(0,0);
      inputChar = ' '; //reset
    }
  }
void status_report(){
   String x = " ";
  for (int i = 0; i < corridors.size(); ++i)
  {
     Serial.println(String(corridors[i].get_corr_id())); 
     Serial.println(String(corridors[i].is_sub()));
  }

  for (int i = 0; i < rooms.size(); ++i)
  {
     Serial.println(String(rooms[i].get_id())); 
     Serial.println(String(rooms[i].get_corr_id())); 
     Serial.println(String(rooms[i].get_objects()));
     Serial.println(String(rooms[i].get_door_direction()));
  }
}

//if we are in a subcorridor, the functionality for the 'complete' options xhanges
void complete(){
    if (is_corridor_sub()){
        Serial.println(currentCorridor);
        sub_corridor();
      }
     else{
         corridor(false);
     }
  }

  //creates a corridor instance, and pushes it to the vector
void createCorridorInstance(bool sub, int curr_dir, int prev_dir){
  Corridor c;
  c.set_corr_id(currentCorridor+1);
  c.set_sub(sub);
  c.set_current_direction(curr_dir);
  corridors.push_back(c);
  currentCorridor =  currentCorridor + 1;
}

  //creates a room instance, and pushes it to the vector
void createRoomInstance(bool detected, int direction){
  Room r;
  r.set_id(lastRoom+1);
  r.set_objects(detected);
  r.set_corr_id(currentCorridor);
  r.set_direction(direction);
  rooms.push_back(r);
  lastRoom =  lastRoom + 1;
} 
void corridor(bool sub) {
    if (!sub) {
      createCorridorInstance(false,0,0);
     Serial.println("XCurrently in Corridor " + String(currentCorridor) + "Last Room Visited" + String(lastRoom));
      char inputChar;
    
  while (line_detection() != "WALL") {
    if(Serial.read() != 'X'){
          motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
      }
      else{
        manual_control();
        reflectanceSensors.read(sensor_values);
        }
        }
        Serial.println("I've hit a corner! Manual Navigation Initiated. Press 'Complete' when I'm back on route.");
        manual_control();
      }
    else if (sub){
      createCorridorInstance(true,0,0);
     Serial.println("XCurrently in Sub Corridor " + String(currentCorridor) + "Last Room Visited: " + String(lastRoom));
        rotate(fmod(averageHeading() + 90, 360));  //does a 90
  // use the line detected code for every corner
 
  if (lookForLine()) {
    rotate(fmod(averageHeading() + 180, 360));
    delay(300); //delay
  }
      sub_corridor();
    }
  }

//this border detect is just for the outside room ..
bool borderDetect() 
{
  reflectanceSensors.read(sensor_values);
  if (sensor_values[0] > QTR_THRESHOLD)
  {
    return true;
  }
  else if (sensor_values[5] > QTR_THRESHOLD)
  {

    return true;
  }
  return false;
}


int scan() {
  //taken from ultrasonic_sensor_test example
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  
    duration = pulseIn(echoPin, HIGH);
    // convert the time into a distance
    cm = microsecondsToCentimeters(duration);
    //Serial.print(cm);
   // if (cm <= 30)
  //  {
      return cm;
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

//checks if there is a line infront/behind the zumo to perform 180 turns.
bool lookForLine() {
  bool temp = false;
  for (int i = 0; i < 3; i++) {
    motors.setLeftSpeed(100);
    motors.setRightSpeed(100);
    delay(250);
    if (borderDetect()) {
      temp = true; 
    }
  }
  //reverse back
  for (int i = 0; i < 3; i++) {
    motors.setLeftSpeed(-75);
    motors.setRightSpeed(-75);
    delay(250);
  }
  return temp;
}

//function when travelling down a sub corridor
void sub_corridor() {
  //go forward until we see a wall 
        while (line_detection() != "WALL") {
            if(Serial.read() != 'X'){
          motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
      }
      else{
        manual_control();
        reflectanceSensors.read(sensor_values);
          }
        }
      Serial.println("End of subcorridor, moving to the end.");
      rotate(fmod(averageHeading() + 180, 360)); //does a 180
              while (line_detection() != "WALL") {
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
   }
   
  int previous_move = get_previous_move();
   char prev, inputChar;
  if (previous_move = 1) {
    prev = 'A'; //1 is left
   }
    else{
      prev = 'D'; // 0 is right
     }

     while (inputChar != prev){
      if (Serial.available() > 0)
      {
        inputChar = Serial.read();
      }
      Serial.println("You cannot turn back!");

     }
     if (inputChar == prev){
      switch (inputChar)
      {
        case 'a': case 'A': rotate(fmod(averageHeading() - 93, 360));lastMove = 1;break;
        case 'd': case 'D': rotate(fmod(averageHeading() + 93, 360));lastMove = 0;break; 
      }
      corridor(false); 
     }       
  }

void outside_room() {
  int direction = 1; //direction of the door, default turn value is left so we set to 1.
  rotate(fmod(averageHeading() + 90, 360));
  if (lookForLine()) {
    rotate(fmod(averageHeading() + 180, 360)); //does a 90
    direction = 0; //on the right
  }
    moveandscan(direction);
}

//moves forward and scans the room for objects
bool moveandscan(int direction) {
        //4 turns does a 360
  motors.setLeftSpeed(200);
  motors.setRightSpeed(200);
  delay(200);
  bool foundsomeone = false;
  for (int i = 0; i < 8;i++) {
    rotate(fmod(averageHeading() + 20, 360)); //does 20 degree turns 
      int distance = sonar.ping_cm();
      delay(20);
      if((distance <= MAX_DISTANCE) && (distance > 0)){
      foundsomeone = true;
      i = 20;
          } 
      }
  if (foundsomeone){
    createRoomInstance(foundsomeone, direction);
    Serial.println("OBJECT/PERSON DETECTED in room" + String(lastRoom) + " corridor: " + String(currentCorridor) + "Please send someone to collect him!");
    manual_control();
  }
    else {
      Serial.println("Nothing detected.");
    }    
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

//this function rotates the zumo based on degrees passed in by the user (i.e. 90)
//this was heavily inspired from the compass example
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
  delay(400);
}


