#include "Arduino.h"
#include "LoRaWan_APP.h"


/* ===== LoRa parameters ===== */
#define RF_FREQUENCY        868000000  // Hz (EU868)
#define TX_OUTPUT_POWER     14         // dBm
#define LORA_BANDWIDTH      0          // 0:125kHz
#define LORA_SPREADING_FACTOR 10         // SF10: good range/speed balance for 5 km
#define LORA_CODINGRATE     4          // 4/8: max error correction for long range
#define LORA_PREAMBLE_LENGTH 8
#define LORA_SYMBOL_TIMEOUT 0
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false

uint8_t txBuffer[32];
uint32_t packetCounter = 0;

/* ===== Radio callbacks ===== */
void onTxDone(void)
{
  Serial.println("✅ TX done");
}

void onTxTimeout(void)
{
  Serial.println("❌ TX timeout");
}

static RadioEvents_t RadioEvents;

void setup()
{
  Serial.begin(115200);
  delay(2000);

  Serial.println("CubeCell LoRa Sender");

  /* Initialize radio callbacks */
  RadioEvents.TxDone = onTxDone;
  RadioEvents.TxTimeout = onTxTimeout;

  Radio.Init(&RadioEvents);

  /* Configure LoRa */
  Radio.SetChannel(RF_FREQUENCY);
  Radio.SetTxConfig(
    MODEM_LORA,
    TX_OUTPUT_POWER,
    0,
    LORA_BANDWIDTH,
    LORA_SPREADING_FACTOR,
    LORA_CODINGRATE,
    LORA_PREAMBLE_LENGTH,
    LORA_FIX_LENGTH_PAYLOAD_ON,
    true,
    0,
    0,
    LORA_IQ_INVERSION_ON,
    3000
  );

  Serial.println("LoRa radio initialized");
}

void loop()
{
  /* Prepare payload */
  sprintf((char*)txBuffer, "Message sent %d", packetCounter++);
  
  Serial.println("------------------------");
  Serial.print("Sending: ");
  Serial.println((char*)txBuffer);

  /* Send packet */
  Radio.Send(txBuffer, strlen((char*)txBuffer));

  /* Process radio IRQ and wait before next transmission */
  Radio.IrqProcess();
  delay(5000);
}
