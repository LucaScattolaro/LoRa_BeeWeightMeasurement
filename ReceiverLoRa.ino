#include "Arduino.h"
#include "LoRaWan_APP.h"

/* ================= LoRa Parameters ================= */
#define RF_FREQUENCY        868000000  // Hz (EU868)
#define TX_OUTPUT_POWER     14         // dBm (not used, but required)
#define LORA_BANDWIDTH      0          // 125 kHz
#define LORA_SPREADING_FACTOR 7
#define LORA_CODINGRATE     1          // 4/5
#define LORA_PREAMBLE_LENGTH 8
#define LORA_SYMBOL_TIMEOUT 0
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false

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
  // Nothing needed here
}
