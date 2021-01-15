#include <LiquidCrystal.h>
 
// Pins
const int d_pin = 8;
const int u_pin = 9;
 
// Dimensions of LCD display
const int lcd_width = 16;
const int lcd_height = 2;
 
#include <LiquidCrystal.h>

// LCD object
LiquidCrystal lcd(12, 11, 2, 3, 4, 5);

// make a tree obstacle
byte tree[8] = {
B00000,
B00100,
B01110,
B11111,
B01110,
B11111,
B00100,
B00100
};

//create an array to store whether or not a tree exists at each of the 16 positions
int obs[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //initally, no trees

//row labels
const int top = 0;
const int bot = 1;
  
void setup() {

  Serial.begin(9600);

  lcd.begin(16,2);
  lcd.clear();

  lcd.createChar(1, tree); //create a tree

}

void loop() {
  //update the screen by moving the obstacle course 1 space to the left
  for(int i=0; i<15; i++){
    //update the array
    obs[i] = obs[i+1];
    //reposition obstacles
    lcd.setCursor(i, bot);
    if (obs[i] == 1){ //place obstacle in position
      lcd.write(byte(1));
    }
    else{ //wipe the current position
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

  delay(1000);
}
