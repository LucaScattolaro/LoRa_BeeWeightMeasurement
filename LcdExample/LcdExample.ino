#include <LiquidCrystal.h>

// Pin mapping (adjust to what you actually wired)
#define RS 1
#define E  2
#define D4 3
#define D5 4
#define D6 5
#define D7 6

// Initialize the library
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

void setup() {
  // Initialize LCD (16 columns, 2 rows)
  lcd.begin(16, 2);

  // Print first message
  lcd.setCursor(0, 0);   // column 0, row 0
  lcd.print("Hello Luca!");

  lcd.setCursor(0, 1);   // second row
  lcd.print("CubeCell OK");
}

void loop() {
  // Example: update text every 2 seconds
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Counter:");

  static int counter = 0;
  lcd.setCursor(0, 1);
  lcd.print(counter);

  counter++;

  delay(2000);
}
