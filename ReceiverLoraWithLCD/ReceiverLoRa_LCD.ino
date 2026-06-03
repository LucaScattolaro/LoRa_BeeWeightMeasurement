#include "Arduino.h"
#include "LoRaWan_APP.h"

/* ================= LoRa Parameters ================= */
#define RF_FREQUENCY          868000000  // Hz (EU868)
#define TX_OUTPUT_POWER       14         // dBm (not used, but required)
#define LORA_BANDWIDTH        0          // 125 kHz
#define LORA_SPREADING_FACTOR 10         // SF10
#define LORA_CODINGRATE       4          // 4/8
#define LORA_PREAMBLE_LENGTH  8
#define LORA_SYMBOL_TIMEOUT   0
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON  false

/* ================= HS420561K-32 Display ================= */
// Wiring (each segment through 1 kΩ resistor):
//   Display pin 11 (A)  --[1kΩ]--> GPIO1
//   Display pin  7 (B)  --[1kΩ]--> GPIO2
//   Display pin  4 (C)  --[1kΩ]--> GPIO3
//   Display pin  2 (D)  --[1kΩ]--> GPIO4
//   Display pin  1 (E)  --[1kΩ]--> GPIO5
//   Display pin 10 (F)  --[1kΩ]--> GPIO6
//   Display pin  5 (G)  --[1kΩ]--> GPIO7
//   Display pin  3 (DP) --[1kΩ]--> GPIO8
//   Display pin 12 (D1) ----------> GPIO9
//   Display pin  9 (D2) ----------> GPIO10
//   Display pin  8 (D3) ----------> GPIO11
//   Display pin  6 (D4) ----------> GPIO12

const int segPins[8]  = {GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, GPIO6, GPIO7, GPIO8};
const int digitPins[4] = {GPIO9, GPIO10, GPIO11, GPIO12};

// Segment patterns 0-9 (common cathode: HIGH = segment ON)
//                         A  B  C  D  E  F  G
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

static RadioEvents_t RadioEvents;
static volatile uint32_t packetCount = 0;

/* ================= Display Functions ================= */
void showDigit(int pos, int num, bool dp) {
  for (int i = 0; i < 4; i++) digitalWrite(digitPins[i], HIGH);
  for (int i = 0; i < 7; i++) digitalWrite(segPins[i], patterns[num][i]);
  digitalWrite(segPins[7], dp ? HIGH : LOW);
  digitalWrite(digitPins[pos], LOW);
}

void displayNumber(uint32_t value) {
  uint32_t v = value % 10000;  // cap at 9999
  int d[4] = {
    (int)(v / 1000) % 10,
    (int)(v /  100) % 10,
    (int)(v /   10) % 10,
    (int) v         % 10
  };
  for (int i = 0; i < 4; i++) {
    showDigit(i, d[i], false);
    delay(4);  // ~4 ms per digit → ~60 Hz refresh
  }
}

/* ================= RX Callback ================= */
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
  packetCount++;

  Serial.print("Packet #");
  Serial.print(packetCount);
  Serial.print("  Data: ");
  for (uint16_t i = 0; i < size; i++) Serial.print((char)payload[i]);
  Serial.print("  RSSI: ");
  Serial.print(rssi);
  Serial.print(" dBm  SNR: ");
  Serial.print(snr);
  Serial.println(" dB");

  Radio.Rx(0);
}

void OnRxTimeout(void)
{
  Serial.println("RX timeout");
  Radio.Rx(0);
}

/* ================= Setup ================= */
void setup()
{
  Serial.begin(115200);

  for (int i = 0; i < 8; i++) {
    pinMode(segPins[i], OUTPUT);
    digitalWrite(segPins[i], LOW);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH);
  }

  RadioEvents.RxDone    = OnRxDone;
  RadioEvents.RxTimeout = OnRxTimeout;

  Radio.Init(&RadioEvents);
  Radio.SetChannel(RF_FREQUENCY);
  Radio.SetRxConfig(
    MODEM_LORA,
    LORA_BANDWIDTH,
    LORA_SPREADING_FACTOR,
    LORA_CODINGRATE,
    0,
    LORA_PREAMBLE_LENGTH,
    LORA_SYMBOL_TIMEOUT,
    LORA_FIX_LENGTH_PAYLOAD_ON,
    0,
    true,
    0,
    0,
    LORA_IQ_INVERSION_ON,
    true
  );

  Serial.println("LoRa Receiver ready");
  Radio.Rx(0);
}

/* ================= Loop ================= */
void loop()
{
  // Multiplex the display (16 ms per full cycle) and process radio events
  displayNumber(packetCount);
  Radio.IrqProcess();
}

