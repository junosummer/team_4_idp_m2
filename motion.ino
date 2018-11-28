
extern bool red_mine_close_to_wall;
extern int turn_count;

void turning_left(){
  myMotor1->setSpeed(255);
  myMotor2->setSpeed(0);
  myMotor1->run(FORWARD);
  myMotor2->run(FORWARD);
  
  }

void turning_right(){
  myMotor1->setSpeed(0);
  myMotor2->setSpeed(255);
  myMotor1->run(FORWARD);
  myMotor2->run(FORWARD);
  
  }

void go_straight_simple(){

  myMotor1->setSpeed(255);
  myMotor2->setSpeed(253);
  myMotor1->run(FORWARD);
  myMotor2->run(FORWARD);

  }

void reverse(){
  myMotor1->setSpeed(255);
  myMotor2->setSpeed(253);
  myMotor1->run(BACKWARD);
  myMotor2->run(BACKWARD);
  
  }
  
void meets_red_mine(){
  if (!red_mine_close_to_wall){
    reverse();
    delay(2000);
    turning_left();
    delay(2750);
    go_straight_simple();
    delay(200);
    turning_right();
    delay(2750);
    go_straight_simple();
    delay(1300);
    turning_right();
    delay(2750);
    go_straight_simple();
    delay(200);
    turning_left();
    delay(2750);
    go_straight();
  }
  else{
    u_turn();
    reverse(); 
    delay(5000);
    update_demand_distance_realistic();
  }
}

void u_turn(){
  reverse();
  delay(3500);  
  turn_count++;
  
  if (positive_x_direction)
  {
    turning_left();
    positive_x_direction = false;
  }
  else
  {
    turning_right();
    positive_x_direction = true;
  }
  
  delay(5700);  //turning so it does a 180-degree turn to go back on a snake route
}

void l_turn(){
  reverse();
  delay(2000); 

  if (positive_x_direction)
  {
    turning_right();
//    positive_x_direction = false;
  }
  else
  {
    turning_left();
//    positive_x_direction = true;
  }
  
  delay(3000);  //turning so it does a 90-degree turn
}

void stop_brake(){
    myMotor1->run(BACKWARD);
    myMotor2->run(BACKWARD);
    delay(42);
    myMotor1->run(RELEASE);
    myMotor2->run(RELEASE);
}


void stop_gentle(){
    myMotor1->run(RELEASE);
    myMotor2->run(RELEASE);
}

void reverse_hit_wall()
{
  reverse();
  delay(3400);
}
