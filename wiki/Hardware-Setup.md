# 🏗️ Hardware-Setup & Systemarchitektur

Das System besteht aus zwei wesentlichen Komponenten: der **Inneneinheit** und der **Außeneinheit**. Diese Trennung ist das Herzstück des Sicherheitskonzepts (Split-Design).

## 🏢 Inneneinheit (Sichere Zone)

Die Inneneinheit wird im geschützten Bereich des Hauses montiert und enthält das "Gehirn" der Anlage.

**Bauteile:**
*   **Microcontroller:** ESP32 DevKit (38-Pin Version, ESP-WROOM-32).
*   **Stromversorgung:** HLK-5M05 AC-DC Wandler (230V auf 5V / 5W). *Achtung: Arbeiten an 230V dürfen nur von Fachpersonal durchgeführt werden!*
*   **Level Shifting:** SN74AHCT125N Quad-Bus-Buffer (wandelt 3.3V Signale des ESP32 sicher auf 5V für Peripherie).
*   **Spannungspufferung:** Elektrolytkondensatoren (2x 1000µF, 470µF, 220µF) glätten die Spannungsversorgung, insbesondere bei Lastspitzen des Fingerprint-Sensors.
*   **Schnittstellen:** RJ45 Buchsen zur Verbindung mit der Außeneinheit. Eine Stiftleiste für zukünftige I2C-Erweiterungen (z.B. OLED-Display) ist vorgesehen.

## 🚪 Außeneinheit (Benutzerschnittstelle)

Diese Einheit wird außen an der Haustür montiert und dient lediglich als Interface.

**Bauteile:**
*   **Biometrie:** R503 Kapazitiver Fingerabdrucksensor (UART-Kommunikation).
*   **RFID/NFC:** PN532 Modul im SPI-Modus.
*   **Umwelt:** DHT22 Sensor für Außentemperatur und Luftfeuchtigkeit.
*   **Interaktion:** Zwei Hardware-Taster ("Haustür Button 1 & 2") und ein Türkontakt-Sensor.
*   **Feedback:** Aktiver 5V Summer (Buzzer), angetrieben durch einen BC567 NPN Transistor. Visuelles Feedback über den LED-Ring des R503 Sensors.

---

## 🔌 Verkabelung & Pin-Belegung

Die Verbindung zwischen Innen- und Außeneinheit erfolgt über ein Standard-RJ45-Patchkabel.

**Wichtige Hinweise zur Verkabelung:**
1.  **Kabellänge:** Da über das Kabel sensible UART/SPI-Signale übertragen werden, ist die Kabellänge auf **maximal 30 cm** begrenzt.
2.  **Schirmung:** Es sollte zwingend ein abgeschirmtes Kabel (z.B. CAT6/CAT7) verwendet werden, um Interferenzen zu vermeiden.
3.  **Reihenfolge:** Verbinden Sie Innen- und Außeneinheit zwingend **vor** dem Einschalten.

### Pin-Belegung (Fest kodiert in der YAML)

| Komponente / Schnittstelle | GPIO Pin (ESP32) | Details & Protokoll |
| :--- | :--- | :--- |
| **Fingerprint R503 (RX)** | GPIO 16 | UART (57600 baud) |
| **Fingerprint R503 (TX)** | GPIO 17 | UART (57600 baud) |
| **Fingerprint R503 (Wake)**| GPIO 33 | Sensing/Touch Pin |
| **NFC PN532 (SCK)** | GPIO 18 | SPI Clock |
| **NFC PN532 (MISO)** | GPIO 19 | SPI Master In Slave Out |
| **NFC PN532 (MOSI)** | GPIO 23 | SPI Master Out Slave In |
| **NFC PN532 (CS)** | GPIO 5 | SPI Chip Select |
| **DHT22 Umweltsensor** | GPIO 4 | One-Wire |
| **Buzzer (Summer)** | GPIO 2 | PWM-gesteuert, Invertiert |
| **Button 1 (Klingel)** | GPIO 14 | Input mit internem Pullup |
| **Button 2** | GPIO 27 | Input mit internem Pullup |
| **Türkontakt** | GPIO 32 | Input mit internem Pullup |

---

## 🛠️ Fertigung der Platinen

Die zugehörigen Gerber-Dateien für die Fertigung der Platinen (`Gerber_haustür-gate_PCB_haustür-gate_*.zip`) befinden sich im Hauptverzeichnis des Repositories. Löten Sie die Platinen gemäß den Schaltplänen und bestücken Sie die Komponenten.
