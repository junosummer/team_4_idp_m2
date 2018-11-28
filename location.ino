int pin_y_trig = 6;  // connect ultrasonic sensor x trigger
int pin_y_echo = 7;  // connect ultrasonic sensor x echo
int pin_x_trig = 4;  // connect ultrasonic sensor y trigger
int pin_x_echo = 5;  // connect ultrasonic sensor y echo
int max_distance = 400; // maximum distance detected
int max_timeout = max_distance * 58;  // maximum timeout for ultrasonic sensor
float critical_wall_x_dis = 19; // critical distance to the wall in the x direction
float critical_wall_y_dis = 28; // critical distance to the wall in the y direction
float field_length_x = 240; // field length in the x direction
float field_length_y = 240; // field length in the y direction

int mine_loc_x_calibration = 13;
float mine_loc_y_calibration[8] = {8, 5.5, 2.5, -0.5, -4, -7, -9.5, -12.5};

int critical_mine_x_loc = 60;

bool red_mine_close_to_wall = false;

extern int loc[];  // location
float EMA_NONE=500.0;
float loc_x_ema_value = EMA_NONE;

#include <Ultrasonic.h>



Ultrasonic ultrasonic_x(pin_x_trig, pin_x_echo, max_timeout);
Ultrasonic ultrasonic_y(pin_y_trig, pin_y_echo, max_timeout);

float exp_moving_avg(float curr_reading, float ema_value){
  float ema_eta = 0.001;
  if (ema_value == EMA_NONE){
    return curr_reading;
  }
  ema_value = (1-ema_eta)*curr_reading + ema_eta*ema_value;
  return ema_value;
}

void get_location(int loc[]) {
  loc[0] = ultrasonic_x.Ranging(CM);
  loc[1] = ultrasonic_y.Ranging(CM);
  
} 

void print_location(){
  String comma = ", ";
//  get_location(loc);
  String curr_loc = loc[0] + comma + loc[1];
  Serial.println("Current location: (" + curr_loc + ")");
}

void print_mine_location(int ldr_idx)
{
  //print location in the required coordinate
  get_location(loc);
  float mine_loc_x = 0;
  float mine_loc_y = 0;
  if (positive_x_direction){
    mine_loc_y = loc[1] - mine_loc_y_calibration[ldr_idx];
    mine_loc_x = field_length_x - loc[0] + mine_loc_x_calibration;
  }
  else{
    mine_loc_x = loc[0] - mine_loc_x_calibration;
    mine_loc_y = field_length_y - loc[1] + mine_loc_y_calibration[ldr_idx];
  }

  red_mine_close_to_wall = false;
  
  String comma = ", ";
//  get_location(loc);
  String mine_loc = mine_loc_x + comma + mine_loc_y;
  Serial.println("Detected mine at location: (" + mine_loc + ")");
  if ((loc[0] -mine_loc_x_calibration) < critical_mine_x_loc){
    red_mine_close_to_wall = true;
  }
}

bool outside_search_zone(){
//  get_location(loc);
  bool close_to_y_wall = ((abs(loc[0]-field_length_x) < critical_wall_x_dis) || (abs(loc[0]) < critical_wall_x_dis));
  bool close_to_x_wall = ((abs(loc[1]-field_length_y) < critical_wall_y_dis) || (abs(loc[1]) < critical_wall_y_dis));
  if (close_to_y_wall || close_to_x_wall){
    Serial.println("GONNA HIT THE BLOODY WALL");}
  return (close_to_y_wall || close_to_x_wall);
 } 

 bool meandering_turning_point(){
//  print_location();
//  get_location(loc);
  loc_x_ema_value = exp_moving_avg(loc[0],loc_x_ema_value);
  bool close_to_y_wall = ((abs(loc_x_ema_value) < critical_wall_x_dis)); // || (abs(loc_x_ema_value-field_length_x) < critical_wall_x_dis)
  if (close_to_y_wall){
    Serial.println("GONNA HIT THE BLOODY WALL");}
  return (close_to_y_wall);
  
 }
