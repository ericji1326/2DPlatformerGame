#include <LiquidCrystal.h>
 
// Pins
const int d_pin = 8;
const int u_pin = 9;
 
// Dimensions of LCD display
const int lcd_width = 16;
const int lcd_height = 2;
 
// LCD object
LiquidCrystal lcd(12, 11, 2, 3, 4, 5);
 
// Create a LCD character 
byte person[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B01010,
  B01010,
  B01010,
};
 
// Character's position
int person_col = 3; //character starts in the 3rd col
const int bot = 1;
const int top = 0;
 
// Button states
int d_prev = HIGH;
int u_prev = HIGH;
 
void setup()
{
  // Set pullups on buttons
  pinMode(d_pin, INPUT_PULLUP);
  pinMode(u_pin, INPUT_PULLUP);
  
  // Initialize and clear LCD 
  lcd.begin(16, 2);
  lcd.clear();
  
  // Register custom character
  lcd.createChar(0, person); //create only the person

  //initial position of character
  lcd.setCursor(person_col, bot);
  lcd.write(byte(0));
}
 
void loop()
{
  
  // Look for left button press and move character down
  int d_state = digitalRead(d_pin);
  if ( (d_prev == HIGH) && (d_state == LOW) ) {
    //clear the top position
    lcd.setCursor(person_col, top);
    lcd.print(" ");
    //put person on bottom row
    lcd.setCursor(person_col, bot);
    lcd.write(byte(0));
  }
  
  // Look for right button press and move character up
  int u_state = digitalRead(u_pin);
  if ( (u_prev == HIGH) && (u_state == LOW) ) {
    //clear bottom position
    lcd.setCursor(person_col, bot);
    lcd.print(" ");
    //put person on top row
    lcd.setCursor(person_col, top);
    lcd.write(byte(0));
  }
  
  // Remember previous button states
  d_prev = d_state;
  u_prev = u_state;
  
  delay(20);
}
