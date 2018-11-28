extern int loc[];
extern float speedLeft;
extern float speedRight;
extern int demand_distance;
extern int demand_sensor_value;
extern bool positive_x_direction;
extern const int car_width;
extern float field_length_y;
extern float field_length_y; 
extern float critical_wall_y_dis; // critical distance to the wall in the y direction
extern float field_length_y; // field length in the y direction

float white_line_to_wall_perpendicular = 45;
float white_line_to_wall_parallel = 24;

float white_line_width = 5;

extern int turn_count;

void start()
{
  //start moving
  myMotor1->setSpeed(250);
  myMotor2->setSpeed(255);
  myMotor1->run(FORWARD);
  myMotor2->run(FORWARD);
}

void go_straight(){
  //find speedLeft and speedRight
  wall_parallel_control(demand_sensor_value, positive_x_direction);
  // write out to the motors
  myMotor1->setSpeed(speedRight);
  myMotor2->setSpeed(speedLeft);
  myMotor1->run(FORWARD); // test whether this is needed.
  myMotor2->run(FORWARD);
}

void wall_parallel_control(int demand_sensor_value, bool positive_x_direc) {
    
  int diff = loc[1] - demand_sensor_value;
  
  if (development)
  {
    print_location();
    Serial.println(demand_sensor_value);
    Serial.println(diff);
  }
    
  //filter large value  
  if (abs(diff) > 10){
    diff = 0;
  }
  
  if (development)
    Serial.println(diff);
    

  // Make the robot turn to its proper orientation
  diff = map(diff, -50, 50, -255, 255); // test to find the diff low-high boundaries
  
  if (diff > 0) {
    speedLeft = 255;
    speedRight = 250 - abs(diff);
  } 
  else {
    speedLeft = 255 - abs(diff);
    speedRight = 250;
  }
}

void update_demand_distance_ideal()
{
  demand_distance += car_width;
  
  if (positive_x_direction)
  {
  demand_sensor_value = demand_distance;
  }
  else
  {
    demand_sensor_value = field_length_y - demand_distance;
  }
}

void update_demand_distance_realistic()
{
  get_location(loc);
  int loc1 = loc[1];
  get_location(loc);
  int loc2 = loc[1];
  get_location(loc);
  int loc3 = loc[1];
  demand_sensor_value = max(max(loc1,loc2),loc3);
//  Serial.println(demand_sensor_value);
}

bool need_to_return(){
  bool need = false;
  unsigned long run_time;
  run_time = millis();
  
  // time reason
  if (run_time > 440000){
    need = true;
    Serial.println("Return due to time limit");
  }
  
  // location reason
  if (mode == 4) //already in turing mode
  {
    if (positive_x_direction && ((demand_sensor_value + car_width) > (field_length_y - critical_wall_y_dis)))
    {
      need = true;
      Serial.println("Return due to next search is outside");
    }
    if (!positive_x_direction && ((demand_distance + car_width) < critical_wall_y_dis))
    {
      need = true;
      Serial.println("Return due to next search is outside");
    }
  }
  
  return need;
}

void return_to_start_box(){
  bool turned = false;
  go_straight_simple();
  while(!turned){
    get_location(loc);
    if(meandering_turning_point()){
      l_turn();
      turned = true;
    }
  }
  get_location(loc);
  demand_distance = loc[1];
//  go_straight_simple();
  if(positive_x_direction){
    turned = false;
    while(!turned){
      go_straight_simple();
      get_location(loc);
      if(meandering_turning_point()){
        l_turn();
        turned = true;
      }
    }
  }
  get_location(loc);
  go_straight_simple();
  while (loc[0] > 30){
    go_straight_simple();
    get_location(loc);
  }
}

bool check_mine_detection_enable()
{
  bool on_parallel_white_line;
  bool on_prependicular_white_line;

  //on_parallel_white_line = (abs(demand_sensor_value - white_line_to_wall_parallel) < (car_width / 2)) || (abs((field_length_y - demand_sensor_value) - white_line_to_wall_parallel) < (car_width / 2));
  on_parallel_white_line = abs(demand_sensor_value <45) || (abs(demand_sensor_value > 195));
  on_prependicular_white_line = (abs(loc[0]-white_line_to_wall_perpendicular) < white_line_width);
  
  if (on_parallel_white_line || on_prependicular_white_line)
  {
    if (development)
      Serial.println("On white line, disable mine detection");
    return false; //disabled
  }
  else if (turn_count == 1 && loc[0] < 80){
    return false;
  }
  else
  {
    return true; //enabled
  }
}
