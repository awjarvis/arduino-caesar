#include <LiquidCrystal.h> 

int Contrast = 30; // Sets contrast, needs to be 30 or it will act a little odd
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Assigns pins to LCD
const int buttonPin1 = 8; // left button
const int buttonPin2 = 7; // right button
int lock = 0; // Button pressed var

String password = "green"; // Needs to be no greater than 8 letters; FIX will later be input by user

void setup(){ // Single iteration setup code
  Serial.begin(9600); // This just needs to be here lol
  analogWrite(6, Contrast); // Sets LCD Contrast
  lcd.begin(16, 2); // Tells the program that the LCD is 16x2
  pinMode(buttonPin, INPUT_PULLUP); // This needs to be here for button
} 

void loop(){ // This code will always run in a loop, every millisecond
// ADD USER INPUT FUNCTION HERE!!

  int sensorValue = analogRead(A0); 
  int val = (sensorValue / 43) + 1;

  // This password check doesn't need to be here if we implement the user input program
  if (password.length() > 8){ // Password > 8 error handling
    lcd.setCursor(0, 0);
    lcd.print("Error: greater");
    lcd.setCursor(0, 1);
    lcd.print("than 8 letters");
  }
  // Only update LCD if the value has changed
  else {
    lcd.clear();  // Clear the display (do this every time)
    lcd.setCursor(0, 0); // Sets print to the first row
    lcd.print(password);

    lcd.setCursor(0, 1); // Sets print to the second row
    lcd.print("Shift = " + String(val)); //Prints the active shift change (have to convert int to str)
  }

  if (digitalRead(buttonPin) == LOW && lock == 0){ // Checks if button is pressed
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(shiftString(password, val));
    lcd.setCursor(0, 1);
    expire(); // Custom animation function I wrote
  }

  delay(100); // Short delay to ensure stability (1/10 of a second)
}

String shiftString(String str, int shift) { // need to test to see if it works
  String shifted = "";
  for (int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    if (isAlpha(c)) {
      char base = isUpperCase(c) ? 'A' : 'a';
      c = ((c - base + shift) % 26 + 26) % 26 + base;
    }
    shifted += c;
  }
  return shifted;
}

// ADD ANIMATION FUNCTION FOR PRINTED DECRYPTION

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
