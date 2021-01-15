#include <LiquidCrystal.h>
 
// Pins
const int u_pin = 9;
const int reset_pin = 8;
const int buzz_pin = 13;
 
// Dimensions of LCD display
const int lcd_width = 16;
const int lcd_height = 2;
 
// LCD object
LiquidCrystal lcd(12, 11, 2, 3, 4, 5);
 
// Create a LCD characters 
byte person[8] = {
  B00000,
  B00100,
  B01110,
  B10101,
  B00100,
  B01010,
  B01010,
  B01010,
};

// make a tree obstacle
byte tree[8] = {
B00100,
B01110,
B11111,
B01110,
B11111,
B00100,
B00100,
B00100
};

byte skull[8] = {
  B01110,
  B11111,
  B10101,
  B11111,
  B01110,
  B00000,
  B01110,
  B00000
};

//LCD rows
const int bot = 1;
const int top = 0;
 
// Character's initial position
int person_col = 3; //character starts in the 3rd col

//create an array to store whether or not a tree exists at each of the 16 positions
int obs[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //initally, no trees

//initial obstacle speed
int obs_speed = 600;
 
// Button states
int u_prev = HIGH;
int reset_prev = HIGH;

//reset function
void(* resetFunc) (void) = 0; 

//points
int num_pts = 0;

//losing message
char lose_msg[26] = "YOU LOSE!                ";
//print the score
char score_msg[] = "SCORE: ";

//starting chime
float start_melody[] = {349.23, 440.0, 523.25, 698.46, 880.0, 1046.5};

//losing chime
float end_melody[] = {1046.5, 830.61, 698.46, 523.25, 415.30, 349.23};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {7,7,7,7,7,4};
 
void setup()
{
  Serial.begin(9600);
  for (int thisNote = 0; thisNote < 6; thisNote++) {
          int noteDuration = 1000/noteDurations[thisNote];
          tone(buzz_pin, start_melody[thisNote],noteDuration);
          int pauseBetweenNotes = noteDuration;
          delay(pauseBetweenNotes);
          noTone(12);
        }
  
  // Set pullups on buttons
  pinMode(u_pin, INPUT_PULLUP);
  pinMode(reset_pin, INPUT_PULLUP);
  pinMode(buzz_pin, OUTPUT);
  
  // Initialize and clear LCD 
  lcd.begin(16, 2);
  lcd.clear();
  
  // Register custom character
  lcd.createChar(0, person); //create person
  lcd.createChar(1, tree); //create tree
  lcd.createChar(2, skull);

  //initial position of character
  lcd.setCursor(person_col, bot);
  lcd.write(byte(0));

  //initialize score
  lcd.setCursor(13, top);
  lcd.print(num_pts);
  
}
 
void loop(){
  
  //CHARACTER MOVEMENT
  //default position is on bottom
  bool jump = false;
  lcd.setCursor(person_col, top);
  lcd.print(" ");
  lcd.setCursor(person_col, bot);
  lcd.write(byte(0));
  
  // Look for right button press and move character up
  int u_state = digitalRead(u_pin);
  if ( (u_prev == HIGH) && (u_state == LOW) ) {
    //clear bottom position
    lcd.setCursor(person_col, bot);
    lcd.print(" ");
    //put person on top row
    lcd.setCursor(person_col, top);
    lcd.write(byte(0));
    for(int t=800; t<1800; t+=10){
    tone(buzz_pin, t);
    delay(1);
    }
    noTone(buzz_pin);
    jump = true;
  }

  //reset the game when reset button is pressed
  int r_state = digitalRead(reset_pin);
  if ( (reset_prev == HIGH) && (r_state == LOW) ) {
        resetFunc();
  }
  
  // Remember previous button states
  u_prev = u_state;
  reset_prev = r_state;
  
  delay(20);
  //OBSTACLE MOVEMENT
  //update the screen by moving the obstacle course 1 space to the left
  for(int i=0; i<15; i++){
    //update the array
    obs[i] = obs[i+1];
    //reposition obstacles
    lcd.setCursor(i, bot);
    if (obs[i] == 1){ //place obstacle in position
      lcd.write(byte(1));

      //if tree goes past the person increment points
      if (i == 2){
        num_pts++;
        lcd.setCursor(13, top);
        lcd.print(num_pts);
      }

      //if character crashes into obstacle, end game
      if (i == person_col && jump == false){
        lcd.clear();
        for (int thisNote = 0; thisNote < 6; thisNote++) {
          int noteDuration = 1000/noteDurations[thisNote];
          tone(buzz_pin, end_melody[thisNote],noteDuration);
          int pauseBetweenNotes = noteDuration;
          delay(pauseBetweenNotes);
          noTone(12);
        }
        lcd.setCursor(16,1);
        for(int i=0; i<26; i++){
          lcd.print(lose_msg[i]);
          delay(150);
          lcd.scrollDisplayLeft();
        }
        
        while( (reset_prev == HIGH) && (r_state == HIGH) ){
          r_state = digitalRead(reset_pin);
          if ( (reset_prev == HIGH) && (r_state == LOW) ){
            resetFunc();
          }
          lcd.clear();
          delay(400);
          lcd.setCursor(4, top);
          lcd.print("SCORE: ");
          lcd.setCursor(11, top);
          lcd.print(num_pts);
          lcd.setCursor(8, bot);
          lcd.write(byte(2));
          lcd.setCursor(7, bot);
          lcd.write(byte(2));
          delay(800);
        }
      }
    }
    
    else if(i != person_col){ //wipe the current position
      lcd.print(" ");
    }
  }
  
  //generate a random 1 or 0 to determine if a new obstacle appears at position 15
  int new_obs = random(0,2);
  
  
  lcd.setCursor(15, bot);
  if (new_obs == 1 && obs[14] == 0) { //place obstacle in position
    obs[15] = 1;
    lcd.write(byte(1));
  }
  else{ //no obstacle
    obs[15] = 0;
    lcd.print(" ");
  }

  //speed up obstacle mvmt every 10 pts
  if (num_pts > 0 && num_pts % 10 == 0){
    obs_speed -= 25;
  }

  delay(obs_speed);
  
}
