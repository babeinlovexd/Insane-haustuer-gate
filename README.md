<div align="center">
  <h1>🚪 Insane Haustür Gate</h1>
  <p><i>Smarte Zutrittskontrolle mit biometrischer und NFC-Validierung für Home Assistant (via ESPHome)</i></p>
</div>

<hr>

## 📋 Projektbeschreibung

Das **Haustür Gate** ist eine maßgeschneiderte Sicherheitslösung für moderne Smart Homes, die eine zuverlässige Brücke zwischen physischer Sicherheit und digitaler Steuerung schlägt.
Das System ermöglicht den schlüssellosen Zugang mittels Fingerabdruck oder NFC-Chips und ist nativ in Home Assistant eingebunden, um maximale Flexibilität bei Automationen zu bieten.

Ein zentraler Aspekt des Designs ist die **physische Trennung von Logik und Interaktion**:
Während die Außeneinheit lediglich als Terminal für Sensoren und Taster dient, verbleiben alle sicherheitsrelevanten Komponenten – einschließlich der Stromversorgung und des ESP32-Controllers – geschützt in der Inneneinheit.
Diese Architektur verhindert effektiv Manipulationsversuche von außen und gewährleistet durch die Verbindung via RJ45-Patchkabel eine stabile Signalübertragung über längere Distanzen.

---

## 🏗️ Systemarchitektur

Das System basiert auf einer Split-Hardware-Architektur, um Funktionalität und Sicherheit im Außenbereich zu gewährleisten:

* 🏠 **Inneneinheit:** Fungiert als Steuerzentrale mit einem ESP32 DevKit, dem AC-DC Wandler (HLK-5M05) und dem Logikpegelwandler.
  Für zukünftige Erweiterungen ist eine **Stiftleiste für Upgrades** integriert, die den einfachen Anschluss von I2C-Komponenten wie OLED-Displays ermöglicht.
* 🚪 **Außeneinheit:** Dient als Benutzerschnittstelle und beherbergt den biometrischen R503 Sensor, das PN532 NFC-Modul sowie Umwelt- und Interaktionselemente.
* 🔌 **Konnektivität:** Beide Einheiten sind über **zwei Standard-RJ45-Patchkabel** miteinander verbunden, was eine einfache Installation und Wartung ermöglicht.

---

## 🛠️ Stückliste (Bill of Materials)

| Komponente | Spezifikation | Menge |
| :--- | :--- | :---: |
| **ESP32 DevKit** | 38-Pin Version (ESP-WROOM-32) | 1 |
| **R503 Sensor** | Kapazitiver Fingerabdrucksensor (UART) | 1 |
| **PN532 Modul** | NFC/RFID Controller (SPI-Mode) | 1 |
| **DHT22** | Temperatur- & Luftfeuchtigkeitssensor | 1 |
| **HLK-5M05** | AC-DC Wandler (230V auf 5V / 5W) | 1 |
| **SN74AHCT125N** | Quad-Bus-Buffer (Logic Level Shifter 3.3V auf 5V) | 1 |
| **Buzzer** | Aktiver 5V Summer | 1 |
| **BC567** | NPN Transistor (Buzzer-Treiber) | 1 |
| **Elektrolytkondensator** | **1000µF (Glättung & Pufferung)** | **2** |
| **Elektrolytkondensator** | 470µF / 220µF | je 1 |
| **Widerstand** | 1kΩ (Basiswiderstand) | 1 |
| **RJ45 Buchsen** | Printmontage für Patchkabel-Link | 4 |

---

## ⚙️ Home Assistant & Software-Funktionen

Dank der ESPHome-Integration bietet das System eine intuitive Verwaltung direkt über das Home Assistant Dashboard:

* ☝️ **Biometrische Registrierung:** Über den Button "Neuen Finger anlernen" wird der Enrollment-Prozess gestartet. Das System verwaltet bis zu 200 Fingerabdrücke und vergibt IDs automatisch.
* 💳 **NFC-Management:** Unterstützt das Hinzufügen von UIDs über einen Lernmodus, der entweder per Software-Button oder durch ein dediziertes Master-Tag aktiviert wird.
* 👤 **Dynamische Namensspeicherung:** Über das Textfeld "Name für neuen Zugang" in Home Assistant kann der Name einer Person eingetippt werden. Drückt man anschließend auf "Anlernen" oder legt im NFC-Lernmodus ein Tag auf, wird der Name lokal auf dem Gate gespeichert und mit dem Tag/Fingerabdruck verknüpft. Sobald die Person später die Tür öffnet, wird der Name im Sensor "Letzter Zugang" angezeigt. Das Textfeld leert sich nach dem Speichern automatisch.
* 🔘 **Benutzer-Interaktion:** Zwei programmierbare Buttons (Haustür Button 1 & 2) können für beliebige HA-Aktionen wie Klingelfunktionen oder Lichtsteuerung genutzt werden.
* 🔊 **Status-Feedback:** Akustische Signale über den Buzzer sowie visuelles Feedback über die Aura-LED des R503 Sensors informieren über den Erfolg oder Fehler eines Zutrittsversuchs. Im Ruhezustand leuchtet die LED dauerhaft blau. Bei Verbindungsverlust zum WLAN pulsiert die LED rot.
* 🌦️ **Umweltdaten:** Der integrierte DHT22-Sensor liefert Außentemperatur und Luftfeuchtigkeit direkt an Home Assistant (inkl. anpassbarem Offset).

---

## 🚀 Installation & Konfiguration

Für die Inbetriebnahme sind folgende Schritte erforderlich:

1. **Hardware-Setup:** Verbinden Sie die Innen- und Außeneinheit über die RJ45-Schnittstellen, bevor Sie die Netzspannung (230V) an das HLK-5M05 Modul anlegen.
2. **Secrets anlegen:** Erstellen Sie eine `secrets.yaml` Datei in Ihrem ESPHome-Verzeichnis mit den notwendigen Zugangsdaten:
   ```yaml
   wifi_ssid: "IHR_WLAN_NAME"
   wifi_password: "IHR_WLAN_PASSWORT"
   wifi_ap_password: "FALLBACK_AP_PASSWORT"
   api_encryption_key: "IHR_API_KEY"
   ota_password: "IHR_OTA_PASSWORT"
   master_tag_id: '"DE-AD-BE-EF"' # Ihre Master-Tag UID
   ```
3. **Master-Tag Konfiguration:** Um den administrativen Zugriff vor Ort zu ermöglichen, wird die UID Ihres persönlichen Master-Tags in der `secrets.yaml` Datei hinterlegt. Das Auflegen des Master-Tags toggelt den Anlernmodus (mit visuellem und akustischem Feedback).
4. **Firmware-Flash:** Kompilieren Sie die Konfiguration (`insane-haustür-gate.yaml`) in ESPHome und führen Sie den ersten Flash-Vorgang via USB durch. Zukünftige Updates können bequem über die OTA-Schnittstelle (Over-the-Air) eingespielt werden.
5. **Home Assistant Integration:** Nach dem erfolgreichen Booten wird das Gerät automatisch als neue Integration erkannt. Fügen Sie es hinzu, um alle Entitäten für das Fingerabdruck-Management, NFC-Whitelist, Buttons und Sensoren zu erhalten.

---

## 🔌 Technische Spezifikationen (Pin-Belegung)

| Schnittstelle | GPIO Pins | Details |
| :--- | :--- | :--- |
| **Fingerprint (UART)** | 16 (RX), 17 (TX), 33 (Sensing) | 57600 baud |
| **NFC Reader (SPI)** | 18 (CLK), 19 (MISO), 23 (MOSI), 5 (CS) | SPI Bus |
| **Umweltsensor** | 4 (DHT22) | Temperatur & Feuchtigkeit |
| **Signalgeber** | 2 (Buzzer) | PWM-gesteuert |
| **Eingänge (Buttons & Kontakt)** | 14, 27 (Buttons), 32 (Kontakt) | Input mit Pullup |

---

<p align="center">
  <i>💡 <b>Hinweis:</b> Die zugehörigen Gerber-Dateien für die Fertigung der Platinen sind im Unterordner als ZIP-Archiv verfügbar.</i>
</p>
