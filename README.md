# LoRa_BeeWeightMeasurement

Point-to-point LoRa communication project for a beehive weight measurement system using the **Heltec CubeCell** (ASR650x) board. Two sketches implement a basic P2P radio link: one sender and one receiver, both operating on the EU868 frequency band.

---

## Project overview

| Sketch | Role | Description |
|---|---|---|
| `SenderLoRa.ino` | Transmitter | Prepares a numbered text payload and broadcasts it every 5 seconds over LoRa. |
| `ReceiverLoRa.ino` | Receiver | Listens continuously for incoming packets and prints the payload, RSSI, and SNR to the serial monitor. |

### SenderLoRa.ino

- Initialises the SX126x radio with the `LoRaWan_APP` library.
- Builds a string payload (`"Message sent <counter>"`) and calls `Radio.Send()` every **5 000 ms**.
- Registers `onTxDone` and `onTxTimeout` callbacks that report transmission status on the serial monitor.

### ReceiverLoRa.ino

- Puts the radio into continuous RX mode with `Radio.Rx(0)` (no timeout).
- On each received packet, `OnRxDone` prints the decoded payload, **RSSI** (dBm), and **SNR** (dB) to the serial monitor, then immediately re-arms the receiver.
- On RX timeout, `OnRxTimeout` restarts listening automatically.

---

## LoRa radio parameters

Both sketches share the same radio configuration so they are compatible out of the box.

| Parameter | Value |
|---|---|
| Frequency | 868 MHz (EU868) |
| TX power | 14 dBm |
| Bandwidth | 125 kHz |
| Spreading factor | SF7 |
| Coding rate | 4/5 |
| Preamble length | 8 symbols |
| IQ inversion | Off |

> **Note:** SF7 / 125 kHz gives the highest data-rate and shortest time-on-air for short-range testing. Increase the spreading factor (e.g. SF10–SF12) for longer range at the cost of slower throughput.

---

## Hardware requirements

- 2 × **Heltec CubeCell Board** (HTCC-AB01 or similar ASR650x module with integrated SX1262)
- USB-C cables for programming and serial monitoring
- Antennas matched to 868 MHz (one per board)

---

## Configuring Arduino IDE for CubeCell

### 1. Install Arduino IDE

Download and install [Arduino IDE 2.x](https://www.arduino.cc/en/software) (recommended) or 1.8.x.

### 2. Add the Heltec CubeCell board package URL

1. Open **File → Preferences** (or **Arduino IDE → Settings** on macOS).
2. In the *Additional boards manager URLs* field, add:
   ```
   https://resource.heltec.cn/download/package_CubeCell_index.json
   ```
3. Click **OK**.

### 3. Install the CubeCell board support package

1. Open **Tools → Board → Boards Manager**.
2. Search for **CubeCell**.
3. Install **Heltec CubeCell Series Dev-boards** (latest version).

### 4. Select the correct board and port

1. Go to **Tools → Board → CubeCell** and select your model, e.g.:
   - **CubeCell-Board (HTCC-AB01)** for the standalone development board.
2. Go to **Tools → Port** and select the COM port that appears when the board is plugged in.

### 5. Configure board options (Tools menu)

| Option | Recommended value |
|---|---|
| LORAWAN_REGION | REGION_EU868 |
| LORAWAN_CLASS | CLASS_A |
| LORAWAN_NETMODE | OTAA |
| AUTO_WAKEUP_BAT | Disbale (for bench testing) |

> For pure P2P LoRa (as used in these sketches) the LoRaWAN region option only affects the frequency plan reference — the radio is driven directly via `Radio.*` calls, not the LoRaWAN stack.

### 6. Install required library

The sketches use `LoRaWan_APP.h`, which is included in the CubeCell board support package installed in step 3. No additional library installation is needed.

### 7. Upload a sketch

1. Open `SenderLoRa.ino` or `ReceiverLoRa.ino` in the IDE.
2. Click **Upload** (→ arrow button).
3. Open **Tools → Serial Monitor**, set baud rate to **115200**, and observe output.

---

## Expected serial output

**Sender:**
```
CubeCell LoRa Sender
LoRa radio initialized
------------------------
Sending: Message sent 0
✅ TX done
------------------------
Sending: Message sent 1
✅ TX done
```

**Receiver:**
```
CubeCell LoRa Receiver P2P
In ascolto LoRa...
📡 Pacchetto ricevuto:
Dati: Message sent 0
RSSI: -45 dBm
SNR: 9 dB
------------------------
```

---

## Customisation

- **Transmission interval:** Change the `delay(5000)` value in `SenderLoRa.ino` (`loop()`).
- **Payload:** Replace the `sprintf` line with actual sensor data (e.g. HX711 load-cell readings for beehive weight).
- **Frequency / SF:** Update the `#define` constants at the top of both sketches (keep them identical on sender and receiver).
