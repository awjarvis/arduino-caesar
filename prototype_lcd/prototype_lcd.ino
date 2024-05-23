#include <LiquidCrystal.h>

int Contrast = 30; // Sets contrast, needs to be 30 or it will act a little odd
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Assigns pins to LCD
const int buttonPin1 = 8; // left button
const int buttonPin2 = 7; // right button

// Main menu vars
int arrVal = 0;
bool menu = true; // Val for the while loop
String menuItem[3] = {"Input Mode", "Caesar Cipher", "other"}; // Menu item array
String password;

//unsigned long lastDebounceTime = 0;
//unsigned long debounceDelay = 50; // Adjust as needed

void setup() { // Single iteration setup code
  Serial.begin(9600); // This just needs to be here lol
  analogWrite(6, Contrast); // Sets LCD Contrast
  lcd.begin(16, 2); // Tells the program that the LCD is 16x2
  pinMode(buttonPin1, INPUT_PULLUP); // This needs to be here for button
  pinMode(buttonPin2, INPUT_PULLUP); // This needs to be here for button
}

void loop() { // This code will always run in a loop

  while (menu) { // Main menu screen
    show("-= Main Menu =-", menuItem[arrVal]);

    if (digitalRead(buttonPin1) == LOW) { // Iterate through the menu items
      if (arrVal == 2) {
        arrVal = 0;
      } else {
        arrVal += 1;
      }
      delay(200);
    }

    if (digitalRead(buttonPin2) == LOW) { // Select current item
      menu = false;
      //if (millis() - lastDebounceTime > debounceDelay) {
        //lastDebounceTime = millis();
      //}
    }
  }

  switch (arrVal) {
    case 0: // Input Mode --------------------------------------------------------------------------------
      // Setup code
      char temp;
      bool input = true;
      String indicator = "^";
      password = "";
      show("Use the dial to", "select letters");
      delay(2900);
      
      while(input){ // Loop
        bool confirm = true;
        /*lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(password + temp);*/

        show(password + temp, indicator);

        int sensorValue = analogRead(A5);
        int val = (sensorValue / 40) + 1;
        temp = 'A' + (val - 1); // Changes int value into letter (A-Z)
        delay(10);

        if (digitalRead(buttonPin1) == LOW ){ //Select
          delay(500);
          password += temp;
          indicator = " " + indicator;
        }
        if (digitalRead(buttonPin2) == LOW ){ //Confirm
          if(password == ""){
            show("Please enter at", "least 1 char");
            delay(2000);
          }
          else{
            while(confirm){
            show("Is this right?", password);
              if (digitalRead(buttonPin1) == LOW ){ // Retry
                confirm = false;
                password = "";
                indicator = "^";
                delay(500);
              }
              if (digitalRead(buttonPin2) == LOW ){ // Confirm
                confirm = false;
                input = false;
                show("Passphrase Set!", "");
                lcd.setCursor(0, 1);
                animatePassword(password);
                delay(1000);
              }
            }
          }
        }
      } //while loop bracket
      menu = true;
      break;

    case 1: // Caesar Cipher -----------------------------------------------------------------------------
      password = "green";
      lcd.clear(); // Clear the display (do this every time)
      lcd.setCursor(0, 0); // Sets print to the first row
      lcd.print(password);
      
      int sensorValue = analogRead(A5);
      int val = (sensorValue / 40) + 1;
      lcd.setCursor(0, 1); // Sets print to the second row
      lcd.print("Shift = " + String(val)); //Prints the active shift change (have to convert int to str)

      if (digitalRead(buttonPin1) == LOW ) { // Checks if button is pressed
        lcd.clear();
        lcd.setCursor(0, 0);
        String shiftedPasswd = shiftString(password, val);
        animatePassword(shiftedPasswd);
        delay(2000);
        lcd.setCursor(0, 1);
        expire(); // Custom animation function

      }

      menu = true;
      break;
    case 2: // Other -------------------------------------------------------------------------------------
      break;

  }
}

void show(String first, String second){ //first is top row and second is bottom row
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(first);
  lcd.setCursor(0, 1);
  lcd.print(second);
  delay(100);
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

void animatePassword(String str) {
  for (int i = 0; i < str.length(); i++) {
    lcd.print(str[i]);
    delay(500);
  }
}

void expire() { // Custom animation function
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
}

/*void getUserInput() {
  String password1 = "";
  lcd.clear();
  lcd.print("Enter Password:");
  while (password1.length() < 8) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      if (isAlphaNumeric(c)) {
        password1 = password1 + c;
        lcd.clear(); // Clear the display (do this every time)
        lcd.setCursor(0, 0); // Sets print to the first row
        lcd.print(password1);
        //animatePassword(password1);
      }
    }
  delay(10); // Adjust delay as needed
  }
  return password1;
}*/
