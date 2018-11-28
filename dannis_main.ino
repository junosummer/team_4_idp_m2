int mode;

const int car_width = 28;
const bool development = false;

int demand_distance = 25;
int demand_sensor_value = 25;
bool positive_x_direction = true;

int loc[] = {0, 0};  // location

float speedLeft = 255;
float speedRight = 255;

const int GREEN_LED_PIN = 53;
const int YELLOW_LED_PIN = 51;
const int RED_LED_PIN = 49;

bool mine_detection_enabled;

int turn_count = 0;

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which port to use
Adafruit_DCMotor *myMotor1 = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(2);

void setup() {
  // put your setup code here, to run once:
  AFMS.begin();
  mode = 1;
  Serial.begin(9600);
  Serial.println("Rebooted");
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(YELLOW_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, HIGH);
  delay(500);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (development)
    Serial.println("main loop start");
  get_location(loc);

  //this function must be put before need_to_return
  if (meandering_turning_point() && mode != 3)
  {
    mode = 4;
  }
  //this function must be put after meandering_turning_point
  if (need_to_return() && mode != 3){
    mode = 5;
  }
  
  switch (mode){
    case 1: // start
      start();
      mode = 2;
      break;
      
    case 2: // go_straight
      go_straight();
      mine_detection_enabled = check_mine_detection_enable();
      if (mine_detection_enabled)
      {
        mine_detection();
      }
      break;
      
    case 3:  // stop
      stop_gentle();
      digitalWrite(GREEN_LED_PIN, LOW);
      break;
      
    case 4:  // turn 180 degrees
      u_turn();
      //update_demand_distance_ideal(); //use car width, cover whole area
      reverse_hit_wall(); 
      update_demand_distance_realistic(); //use sensor value
      mode = 2;
      break;
      
    case 5:  // return to start box
      if (development)
        Serial.println("in case 5");
      digitalWrite(GREEN_LED_PIN, HIGH);
      digitalWrite(YELLOW_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, HIGH);
      return_to_start_box();
      mode = 3;
      delay(500);
      digitalWrite(YELLOW_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, LOW);
      if (development)
        Serial.println("leave case 5");
      break;
      
    default:
      go_straight();
  }

}
