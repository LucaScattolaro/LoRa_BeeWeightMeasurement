// HS420561K-32 — common-cathode 4-digit 7-segment display
// Seconds counter on CubeCell HTCC-AB02A
//
// HS420561K-32 pinout (from datasheet):
//   Pin 1  = E       Pin 7  = B
//   Pin 2  = D       Pin 8  = D3 (digit 3 cathode)
//   Pin 3  = DP      Pin 9  = D2 (digit 2 cathode)
//   Pin 4  = C       Pin 10 = F
//   Pin 5  = G       Pin 11 = A
//   Pin 6  = D4      Pin 12 = D1 (digit 1 cathode)
//
// Wiring (each segment pin through 1 kΩ resistor):
//   Display pin 11 (A)  --[1kΩ]--> GPIO1
//   Display pin  7 (B)  --[1kΩ]--> GPIO2
//   Display pin  4 (C)  --[1kΩ]--> GPIO3
//   Display pin  2 (D)  --[1kΩ]--> GPIO4
//   Display pin  1 (E)  --[1kΩ]--> GPIO5
//   Display pin 10 (F)  --[1kΩ]--> GPIO6
//   Display pin  5 (G)  --[1kΩ]--> GPIO7
//   Display pin  3 (DP) --[1kΩ]--> GPIO8
//
//   Display pin 12 (D1) ----------> GPIO9
//   Display pin  9 (D2) ----------> GPIO10
//   Display pin  8 (D3) ----------> GPIO11
//   Display pin  6 (D4) ----------> GPIO12

// Segment pins: A, B, C, D, E, F, G, DP
const int segPins[8] = {GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, GPIO6, GPIO7, GPIO8};

// Digit select pins: D1 (leftmost) to D4 (rightmost)
const int digitPins[4] = {GPIO9, GPIO10, GPIO11, GPIO12};

// Segment patterns 0-9 (common cathode: HIGH = segment ON)
//                        A  B  C  D  E  F  G
const byte patterns[10][7] = {
  {1, 1, 1, 1, 1, 1, 0},  // 0
  {0, 1, 1, 0, 0, 0, 0},  // 1
  {1, 1, 0, 1, 1, 0, 1},  // 2
  {1, 1, 1, 1, 0, 0, 1},  // 3
  {0, 1, 1, 0, 0, 1, 1},  // 4
  {1, 0, 1, 1, 0, 1, 1},  // 5
  {1, 0, 1, 1, 1, 1, 1},  // 6
  {1, 1, 1, 0, 0, 0, 0},  // 7
  {1, 1, 1, 1, 1, 1, 1},  // 8
  {1, 1, 1, 1, 0, 1, 1}   // 9
};

unsigned long previousMillis = 0;
int seconds = 0;

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(segPins[i], OUTPUT);
    digitalWrite(segPins[i], LOW);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH);  // HIGH = digit OFF (cathode not grounded)
  }
}

// Show one digit at a given position (0=leftmost, 3=rightmost)
void showDigit(int pos, int num, bool dp) {
  // Turn off all digits first (prevents ghosting)
  for (int i = 0; i < 4; i++)
    digitalWrite(digitPins[i], HIGH);

  // Set segment outputs
  for (int i = 0; i < 7; i++)
    digitalWrite(segPins[i], patterns[num][i]);
  digitalWrite(segPins[7], dp ? HIGH : LOW);

  // Activate digit (LOW = cathode grounded = digit ON)
  digitalWrite(digitPins[pos], LOW);
}

// Multiplex a 4-digit number (0-9999) for one refresh cycle
void displayNumber(int value) {
  int d[4] = {
    (value / 1000) % 10,
    (value /  100) % 10,
    (value /   10) % 10,
     value         % 10
  };
  for (int i = 0; i < 4; i++) {
    showDigit(i, d[i], false);
    delay(4);  // ~4 ms per digit → ~60 Hz refresh
  }
}

void loop() {
  // Increment seconds every 1000 ms
  unsigned long now = millis();
  if (now - previousMillis >= 1000) {
    previousMillis = now;
    seconds++;
    if (seconds > 9999) seconds = 0;
  }

  // Continuously multiplex the display
  displayNumber(seconds);
}
