const int COLOUR_PIN_ARRAY[8] = {A0,A1,A2,A3,A4,A5,A6,A7};
const int COLOUR_THRESHOLD_ARRAY[8][3]={{455, 580, 600}, {410, 590, 610}, {430, 595, 615}, {410, 575, 595}, {475, 625, 645}, {465, 580, 600}, {440, 510, 525}, {410, 560, 580}};
const int COLOUR_ARRAY[8][4]={{476,532,620,708},{431,513,627,719},{451,564,636,741},{429,496,613,708},{497,536,667,745},{488,547,623,721},{463,491,563,684},{428,539,601,704}};

// black 362.00 335.00 358.00 349.00 360.00 370.00 350.00 366.00

int colors[8];
int colors_adjusted[8];
int color;
bool yellow_in_array = false;
bool white_in_array = false;
bool red_in_array = false;
int yellow_loc = 0;
int red_loc = 0;


  
int single_mine_diff(int ldr_index){
  /* Take the analog input pin
   * Return the color seen by the corresponding sensor in int
   * color_code: black-0, red-1, white_2, yellow_3
   */
  float colour_pin = COLOUR_PIN_ARRAY[ldr_index];
  float colour_volt = analogRead(colour_pin);
  if (development)
  {
    Serial.print(colour_volt);
    Serial.print(" ");
  }
  
  // voltage low to high: black red white yellow

  if (colour_volt < COLOUR_THRESHOLD_ARRAY[ldr_index][0]){
    return 0; // black
  }
  
  else if ((colour_volt > COLOUR_THRESHOLD_ARRAY[ldr_index][0]) && (colour_volt < COLOUR_THRESHOLD_ARRAY[ldr_index][1])){
    return 1; // red
  }
  
  else if (colour_volt > COLOUR_THRESHOLD_ARRAY[ldr_index][2]){
    return 3; // yellow
  }
  else {
    return 2; // white
  }
  
}

void red_led(){
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH); 
  delay(3000); 
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
}

void yellow_led(){
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, HIGH); 
  delay(3000); 
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
}

void read_all_colors()
{
  red_in_array = false;
  white_in_array = false;
  yellow_in_array = false;
  for (int i = 0; i < 8; i = i + 1 ) {
    color = single_mine_diff(i);
    colors[i] = color;
//    
//    switch (color)
//    {
//    case 1: 
//      red_in_array = true;
//      break;
//      
//    case 2: 
//      white_in_array  = true;
//      break;
//      
//    case 3:  
//      yellow_in_array  = true;
//      break;
//    }
  }
  if (development)
    Serial.println(" ");
}

void adjust_colors(){
  red_in_array = false;
  white_in_array = false;
  yellow_in_array = false;
  for (int j = 3; j > 0; j--){
    for (int i = 0; i < 8; i = i + 1 ) {
      if (colors[i] == j){
        if (i-1 > -1){
          colors[i-1]=0;  //?colors[i-1]=0:colors[i-1]=j;
        }
        if (i+1 < 8){
          colors[i+1]=0;  //?colors[i+1]=0:colors[i+1]=j;
        }
        switch (j){
          case 3:
            yellow_in_array = true;
            yellow_loc = i;
            break;
          case 2:
            white_in_array = true;
            break;
          case 1:
            red_in_array = true;
            red_loc = i;
            break;
        }
      }
    }
  }
}

void mine_detection() {
  read_all_colors();
  
  if (development)
  {
    for (int i = 0; i < 8; i++){
      Serial.print(colors[i]);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
  
  adjust_colors();
  
  if (development)
  {
    for (int i = 0; i < 8; i++){
      Serial.print(colors[i]);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
  
//  if (yellow_in_array == true || red_in_array == true || white_in_array == true)
  if (yellow_in_array == true || red_in_array == true)
  {
    Serial.println("Detected!");
    stop_brake();
    delay(100);
    
    read_all_colors(); // reread color
    adjust_colors();
    
    if (yellow_in_array)
    {
      Serial.println("Yellow Mine");
      print_mine_location(yellow_loc);
      yellow_led();
      go_straight();
      delay(500);
    }
    
    else if (red_in_array || white_in_array || ((!red_in_array)&&(!yellow_in_array)&&(!white_in_array))){
      Serial.println("IT'S A DANGEROUS BIG FAT RED MINE");
      print_mine_location(red_loc);
      red_led();
      meets_red_mine();
      go_straight();
      delay(500);
    }

//    else if (white_in_array)
//    {
//      Serial.println("I walk the WHITE line");
//    }
    
  }
}
