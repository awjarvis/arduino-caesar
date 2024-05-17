#include <LiquidCrystal.h> // lol

int Contrast = 30; // Sets contrast, needs to be 30 or it will act a little odd
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Assigns pins to LCD
const int buttonPin1 = 8; // left button
const int buttonPin2 = 7; // right button
int lock = 0; // Button pressed var

// Main menu vars
int arrVal = 0;
bool menu = true; //  Val for the while loop
String menuItem[3] = {"Input Mode", "Caesar Cipher", "other"}; // Menu item array

String password; // Needs to be no greater than 8 letters; FIX will later be input by user


void setup(){ // Single iteration setup code
  Serial.begin(9600); // This just needs to be here lol
  analogWrite(6, Contrast); // Sets LCD Contrast
  lcd.begin(16, 2); // Tells the program that the LCD is 16x2
  pinMode(buttonPin1, INPUT_PULLUP); // This needs to be here for button
  pinMode(buttonPin2, INPUT_PULLUP); // This needs to be here for button
} 

void loop(){ // This code will always run in a loop
// ADD USER INPUT FUNCTION HERE!!

  while (menu == true){ // Main menu screen
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("-= Main Menu =-");
    lcd.setCursor(0, 1);
    lcd.print(menuItem[arrVal]);
    delay(100);

    if (digitalRead(buttonPin1) == LOW && lock == 0){ // Iterate through the menu items
      if (arrVal == 2){
        arrVal = 0;
      }
      else{ 
        arrVal += 1;
      }
      delay(200);
    }

    if (digitalRead(buttonPin2) == LOW && lock == 0){ //currently right button doesn't work
      menu = false;
    }
  }

  int sensorValue = analogRead(A5); 
  int val = (sensorValue / 40) + 1;

  switch (arrVal){
    case 0: // Input Mode
      break;
    case 1: // Caesar Cipher
      
      lcd.clear();  // Clear the display (do this every time)
      lcd.setCursor(0, 0); // Sets print to the first row
      lcd.print(password);

      lcd.setCursor(0, 1); // Sets print to the second row
      lcd.print("Shift = " + String(val)); //Prints the active shift change (have to convert int to str)

      if (digitalRead(buttonPin1) == LOW && lock == 0){ // Checks if button is pressed
        lcd.clear();
        lcd.setCursor(0, 0);
        String shiftedPasswd = shiftString(password, val);
        animateShiftedPasswd(shiftedPasswd);
        delay(2000);
        lcd.setCursor(0, 1);
        expire(); // Custom animation function
        
      }
      break;
    case 2: // Other
      break;

  }

  delay(100); // Short delay to ensure stability (1/10 of a second)
}


String shiftString(String str, int shift) { // Works! :)
  String shifted = "";
  for (int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    if (isAlpha(c)) {
      char base = isUpperCase(c) ? 'A' : 'a';
      c = ((c - base - shift) % 26 + 26) % 26 + base;
    }
    shifted += c;
  }
  return shifted;
}

void animateShiftedPasswd(String shiftedPasswd)
{
  for (int i = 0; i < shiftedPasswd.length(); i++)
  {
    lcd.print(shiftedPasswd[i]);
    delay(500);
  }
}

void expire(){ // Custom animation function
    lcd.setCursor(0, 1);
    delay(1000); // one second
    lcd.print(".");
    delay(1000);
    lcd.print(".");
    delay(1000);
    lcd.print(".");  
    delay(1000);
    lcd.print(".");
    delay(1000);
    lcd.print(".");
    delay(1000);
    lcd.print(".");
    delay(1000);
    lcd.print(".");
    delay(1000);
    lcd.print(".");
    delay(1000);
    lcd.print(".");
    delay(1000);
    lcd.print(".");
}
