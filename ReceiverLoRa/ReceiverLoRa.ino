#include "Arduino.h"
#include "LoRaWan_APP.h"
#include <LiquidCrystal.h>

/* ================= LoRa Parameters ================= */
#define RF_FREQUENCY        868000000  // Hz (EU868)
#define TX_OUTPUT_POWER     14         // dBm (not used, but required)
#define LORA_BANDWIDTH      0          // 125 kHz
#define LORA_SPREADING_FACTOR 10         // SF10: good range/speed balance for 5 km
#define LORA_CODINGRATE     4          // 4/8: max error correction for long range
#define LORA_PREAMBLE_LENGTH 8
#define LORA_SYMBOL_TIMEOUT 0
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false

// Pin mapping (adjust to what you actually wired)
#define RS 1
#define E  2
#define D4 3
#define D5 4
#define D6 5
#define D7 6

// Initialize the library
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);


static RadioEvents_t RadioEvents;

/* ================= RX Callback ================= */
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
  Serial.println("📡 Pacchetto ricevuto:");

  Serial.print("Dati: ");
  for (uint16_t i = 0; i < size; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  Serial.print("RSSI: ");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.print("SNR: ");
  Serial.print(snr);
  Serial.println(" dB");

  Serial.println("------------------------");

  /* Restart RX */
  Radio.Rx(0);
}

void OnRxTimeout(void)
{
  Serial.println("⏳ RX timeout");
  Radio.Rx(0);
}

/* ================= Setup ================= */
void setup()
{  
  // Initialize LCD (16 columns, 2 rows)
  lcd.begin(16, 2);
  // Print first message
  lcd.setCursor(0, 0);   // column 0, row 0
  lcd.print("Hello Luca!");
  lcd.setCursor(0, 1);   // second row
  lcd.print("CubeCell OK");

  Serial.begin(115200);
  delay(2000);

  Serial.println("CubeCell LoRa Receiver P2P");

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

  Serial.println("In ascolto LoRa...");
  Radio.Rx(0);
}

/* ================= Loop ================= */
void loop()
{
  /* Required by Heltec CubeCell library to dispatch radio events (RxDone, etc.) */
  Radio.IrqProcess();
}
