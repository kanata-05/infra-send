#include <LiquidCrystal.h>

int infraREAD;
unsigned long startTime = 0;
unsigned long idleTime = 0;
bool signalDetected = false;
String morseCode = "";
String displayText = ""; // Holds the displayed text

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  lcd.begin(16, 2);
  pinMode(A0, INPUT);
  lcd.print("Setup Finished!");
  delay(3000);
  Serial.begin(9600);
}

void loop() {
  infraREAD = analogRead(A0);
  Serial.println(infraREAD);
  if (infraREAD > 0) { // Signal detected
    if (!signalDetected) { // Start of a signal
      signalDetected = true;
      startTime = millis();
    }
  } else if (signalDetected) { // Signal ended
    unsigned long duration = millis() - startTime;

    if (duration >= 100 && duration < 500) {
      morseCode += "."; // Dot
    } else if (duration >= 500) {
      morseCode += "-"; // Dash
    }

    signalDetected = false; // Reset detection
    idleTime = millis();
  }

  // Check for idle time
  if (!signalDetected && millis() - idleTime > 1200 && morseCode != "") {
    if (morseCode == ".....") { // Special case for removing last character
      if (displayText.length() > 0) {
        displayText.remove(displayText.length() - 1); // Remove last character
        Serial.println("Character removed");
        lcd.clear();
        lcd.print(displayText); // Update LCD
      }
    } else { // Regular Morse code translation
      String translated = translateMorse(morseCode);
      displayText += translated; // Add translated character to display text
      lcd.clear();
      lcd.print(displayText); // Display the updated text on LCD
    }

    morseCode = ""; // Clear the buffer
  }
}

// Function to translate Morse code to text
String translateMorse(String morse) {
  if (morse == ".-") return "A";
  if (morse == "-...") return "B";
  if (morse == "-.-.") return "C";
  if (morse == "-..") return "D";
  if (morse == ".") return "E";
  if (morse == "..-.") return "F";
  if (morse == "--.") return "G";
  if (morse == "....") return "H";
  if (morse == "..") return "I";
  if (morse == ".---") return "J";
  if (morse == "-.-") return "K";
  if (morse == ".-..") return "L";
  if (morse == "--") return "M";
  if (morse == "-.") return "N";
  if (morse == "---") return "O";
  if (morse == ".--.") return "P";
  if (morse == "--.-") return "Q";
  if (morse == ".-.") return "R";
  if (morse == "...") return "S";
  if (morse == "-") return "T";
  if (morse == "..-") return "U";
  if (morse == "...-") return "V";
  if (morse == ".--") return "W";
  if (morse == "-..-") return "X";
  if (morse == "-.--") return "Y";
  if (morse == "--..") return "Z";
  return "?"; // Unknown Morse code
}
