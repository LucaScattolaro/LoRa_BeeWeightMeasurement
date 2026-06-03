// LED Blink Example for CubeCell HTCC-AB02A
// Toggles an LED connected to GPIO5

#define LED_PIN GPIO5

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}
