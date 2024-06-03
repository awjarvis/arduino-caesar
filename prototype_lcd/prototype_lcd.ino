#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Assigns pins to LCD
const int buttonPin1 = 8; // left button
const int buttonPin2 = 7; // right button

// Main menu vars
int arrVal = 0;
bool menu = true; // Val for the while loop
bool login = false;
String menuItem[5] = {"Input Ciphertext", "Decrypt Atbash", "Decrypt Caesar", "???", "Logout"}; // Menu item array
String ciphertext; // Global ciphertext var

void setup() { // Single iteration setup code
  Serial.begin(9600); // This just needs to be here lol
  analogWrite(6, 30); // Sets LCD main pin and Contrast
  lcd.begin(16, 2); // Tells the program that the LCD is 16x2
  pinMode(buttonPin1, INPUT_PULLUP); // This needs to be here for button1
  pinMode(buttonPin2, INPUT_PULLUP); // This needs to be here for button2
}

void loop() { // This code will always run

  if(!login) {
    show( " -= Welcome =-", "");
    delay(1900);
    show("Use the dial to", "select 3 numbers");
    delay(2900);
    displayLogin();
  }
  arrVal = 0;
  displayMenu();

  switch (arrVal) {
    case 0: // Input Mode 
      show("Use the dial to", "select letters");
      delay(2900);
      inputMode(); 
      menu = true; // goes back to main menu
      break;

    case 1: // Atbash 
      if (ciphertext != ""){
         atbashMode();
      }
      else{
        show("Please enter a", "cipher first");
        delay(2900);
      }
      menu = true; // goes back to the main menu
      break;

    case 2: // Caesar Cipher
      if (ciphertext != ""){
      show("Use the dial to", "select the key");
      delay(2900);
      caesarMode();
      }
      else {
        show("Please enter a", "cipher first");
        delay(2900);
      }
      menu = true; // goes back to main menu
      break;

    case 3:
      message();
      menu = true;
      break;

    case 4: // Logout
      show("Logging out...", "");
      delay(1000);
      login = false;
      menu = true;
      break;
  }
}

void show(String firstRow, String secondRow){ //first is top row and second is bottom row
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(firstRow);
  lcd.setCursor(0, 1);
  lcd.print(secondRow);
  delay(100);
}

String atbashCipher(String input) {
  String plaintext = ""; // Temporary string for the deciphered message
  for (int i = 0; i < input.length(); i++) { // This goes through all letters of the ciphertext
    char shiftedLetter = input[i]; // Stores the letter that is being shifted
    plaintext += (char)('Z' - (shiftedLetter - 'A')); // Applies the letter swap and adds the letter to the plaintext
  }
  return plaintext; // Outputs the plaintext in its full form
}

String decryptCaesar(String str, int shift) { // Declares the function so it can be called from the main area
  String plaintext = ""; // Temporary string for the deciphered message
  for (int i = 0; i < str.length(); i++) { // This goes through all letters of the ciphertext
    char shiftedLetter = str.charAt(i); // Stores the letter that is being shifted
    shiftedLetter = ((shiftedLetter - 'A' - shift) % 26 + 26) % 26 + 'A';  // Shifts the letter
    plaintext += shiftedLetter; // Adds the shifted letter to plaintext
  }
  return plaintext; // Outputs the plaintext in its full form
}

void displayMenu(){
  while (menu) { // Main menu screen
    show("-= Main Menu =-", menuItem[arrVal]);

    if (digitalRead(buttonPin1) == LOW) { // Iterate through the menu items
      if (arrVal == 4) {
        arrVal = 0;
      } else {
        arrVal += 1;
      }
      delay(200);
    }

    if (digitalRead(buttonPin2) == LOW) { // Select current item
      menu = false;
    }
  }
}

void inputMode(){
  // Setup code
  char temp = "";
  bool input = true;
  String indicator = "^";
  ciphertext = "";
  
  while(input){ // Loop
    bool confirm = true;

    show(ciphertext + temp, indicator);

    int sensorValue = analogRead(A5);
    int val = (sensorValue / 40) + 1;
    temp = 'A' + (val - 1); // Changes int value into letter (A-Z)

    if (digitalRead(buttonPin1) == LOW ){ //Select
      delay(500);
      ciphertext += temp;
      indicator = " " + indicator;
    }
    if (digitalRead(buttonPin2) == LOW ){ //Confirm
      if(ciphertext == ""){
        show("Please enter at", "least 1 char");
        delay(2000);
      }
      else{
        delay(200);
        while(confirm){
        show("Is this right?", ciphertext);
        
          if (digitalRead(buttonPin1) == LOW ){ // Retry
            confirm = false;
            ciphertext = "";
            indicator = "^";
            delay(500);
          }
          if (digitalRead(buttonPin2) == LOW ){ // Confirm
            confirm = false;
            input = false;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Ciphertext Set!");
            animateStr(ciphertext, 1);
            delay(1000);
          }
        }
      }
    }
    delay(10);
  } //while loop bracket
}

void caesarMode(){
  int shift = 0;
  bool temp = true;
  delay(500);
  while (temp) {
    int sensorValue = analogRead(A5);
    shift = (sensorValue / 40) + 1;

    show(ciphertext, "Key = " + String(shift));

    if (digitalRead(buttonPin1) == LOW) { // Apply shift
      lcd.clear();
      lcd.setCursor(0, 0);
      String shiftedPasswd = decryptCaesar(ciphertext, shift);
      animateStr(shiftedPasswd, 0);
      delay(200);
      lcd.setCursor(0, 1);
      expire(); // Custom animation function
    }

    if (digitalRead(buttonPin2) == LOW) { // Exit
      show("Exiting...", "");
      delay(1000);
      temp = false;
    }
    delay(10);
  }
}

void atbashMode(){
  bool temp = true;
  String tempStr;
  delay(500);
  while (temp) {
    show("Ciphertext:", ciphertext);

    if (digitalRead(buttonPin1) == LOW) { // Apply decryption
      lcd.clear();
      animateStr(atbashCipher(ciphertext), 0);
      expire();
    }

    if (digitalRead(buttonPin2) == LOW) { // Exit
      show("Exiting...", "");
      delay(1000);
      temp = false;
    }
    delay(10);
  }
}

void displayLogin(){
  String passkeyStr = "";
  int iterator = 0;

  while(!login){
    int i = (analogRead(A5)/30) + 1;
    show("Login Passkey:", (passkeyStr + String(i)));

    if (digitalRead(buttonPin1) == LOW) { // Select
      passkeyStr += (String(i) + " ");
      delay(200);
      if(iterator == 2){
        if(passkeyStr == "6 23 12 "){
          show("Passkey Accepted", ":)");
          delay(2000);
          login = true;
        }
        else{
          show ("Wrong Passkey", ":(");
          delay (2000);
          passkeyStr = "";
          iterator = 0;
        }
      }
      else {
      iterator += 1;
      }
    }
  }
}

void message(){
  show("Some","secret");
  delay(2900);
  show("message","");
  delay(2900);
}

void animateStr(String str, int r) { // r is the row of the LCD
  lcd.setCursor(0, r);
  for (int i = 0; i < str.length(); i++) {
    lcd.print(str[i]);
    delay(350);
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