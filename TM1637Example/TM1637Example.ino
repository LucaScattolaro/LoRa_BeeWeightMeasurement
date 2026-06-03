
// HS420561K-32 — common cathode 4-digit 7-segment display
// Cube Cell HTCC-AB02A (3.3 V logic)
//
// Wiring used:
//   GPIO1–7  --[1kΩ]--> A–G      (1kΩ keeps segment current ≈1.3 mA,
//   GPIO8    --[1kΩ]--> DP        max cathode sink ≈10 mA — safe for GPIO)
//   GPIO9–12 ----------> D1–D4   (common cathodes; LOW = digit ON)
//   GPIO13   --[220Ω]--> LED anode  (cathode to GND; HIGH = ON)

// Segment pins: A, B, C, D, E, F, G, DP
const int segPins[8] = {1, 2, 3, 4, 5, 6, 7, 8}; // GPIO1–GPIO8

// Digit select pins: D1, D2, D3, D4 (common cathodes)
const int digitPins[4] = {9, 10, 11, 12}; // GPIO9–GPIO12

// Status LED
const int LED_PIN = 13; // GPIO13

// Segment patterns 0–9 (common cathode: 1 = segment ON)
// Column order: A, B, C, D, E, F, G, DP
const byte numbers[10][8] = {
  {1,1,1,1,1,1,0,0}, // 0
  {0,1,1,0,0,0,0,0}, // 1
  {1,1,0,1,1,0,1,0}, // 2
  {1,1,1,1,0,0,1,0}, // 3
  {0,1,1,0,0,1,1,0}, // 4
  {1,0,1,1,0,1,1,0}, // 5
  {1,0,1,1,1,1,1,0}, // 6
  {1,1,1,0,0,0,0,0}, // 7
  {1,1,1,1,1,1,1,0}, // 8
  {1,1,1,1,0,1,1,0}  // 9
};

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(segPins[i], OUTPUT);
    digitalWrite(segPins[i], LOW);   // all segments off
  }
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH); // all digits off (HIGH = cathode not grounded)
  }

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // LED off initially
}

void ledOn()  { digitalWrite(LED_PIN, HIGH); }
void ledOff() { digitalWrite(LED_PIN, LOW);  }

// Display digit 'num' (0–9) at position 'pos' (0 = leftmost).
// Set dp = true to light the decimal point.
void displayDigit(int pos, int num, bool dp = false) {
  // Blank all digits before changing segments (prevents ghosting)
  for (int i = 0; i < 4; i++) digitalWrite(digitPins[i], HIGH);

  // Drive segment outputs
  for (int i = 0; i < 7; i++)
    digitalWrite(segPins[i], numbers[num][i]);
  digitalWrite(segPins[7], dp ? HIGH : LOW); // DP

  // Activate the chosen digit (sink its cathode to GND)
  digitalWrite(digitPins[pos], LOW);
}

// Multiplex a 4-digit integer (0–9999) for one full refresh cycle.
void displayNumber(int value) {
  int digits[4] = {
    (value / 1000) % 10,
    (value /  100) % 10,
    (value /   10) % 10,
     value         % 10
  };
  for (int i = 0; i < 4; i++) {
    displayDigit(i, digits[i]);
    delay(3); // multiplex period per digit
  }
}

void loop() {
  ledOn();
  // displayNumber(1234);
  // delay(2);
  // ledOff();
  // // displayNumber(1234);
  // delay(2);
}
