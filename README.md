<div align="center">
  <h1>🚪 Insane Haustür Gate</h1>
  <p><i>Smarte Zutrittskontrolle mit biometrischer und NFC-Validierung für Home Assistant (via ESPHome)</i></p>
</div>

<hr>

Das **Haustür Gate** ist eine vollumfängliche, smarte Zutrittslösung für Home Assistant. Es kombiniert einen biometrischen Fingerabdrucksensor (R503) mit einem NFC-Lesegerät (PN532), um eine sichere, schlüssellose Türöffnung zu ermöglichen. Die Architektur ist für maximale Sicherheit und Zuverlässigkeit ausgelegt.

## ✨ Features (Hardware & Software)

* **Autonome Funktionalität:** Der ESP32 verwaltet Zugangsdaten (NFC-Whitelist und Fingerabdruck-IDs) lokal in seinem Flash-Speicher (via ESPHome `restore_value: yes`). Selbst bei einem Ausfall des WLANs oder von Home Assistant bleibt die Türöffnungsfunktion erhalten!
* **Physische Sicherheit (Split-Design):** Das System ist in zwei Einheiten aufgeteilt. Die sicherheitsrelevante Logik (ESP32, Stromversorgung, Relais/Türöffner-Ansteuerung) befindet sich in einer **geschützten Inneneinheit**. Die **Außeneinheit** dient nur als Schnittstelle (Sensoren, Taster, LEDs).
* **Datenschutz & Komfort:** Namen von Benutzern können direkt über Home Assistant beim Anlernen eingegeben werden. Das System speichert diese Zuordnung (UID/Finger-ID zu Name) lokal und meldet bei Zutritt den Namen des Benutzers an Home Assistant zurück ("Letzter Zugang").

---

## 🏗️ Systemarchitektur & Hardware

Die Trennung von Innen- und Außeneinheit wird durch eine einfache Verbindung via Standard-RJ45-Patchkabel realisiert. Da hierüber sensible UART/SPI-Signale übertragen werden, ist die Kabellänge auf **maximal 30 cm** begrenzt und es sollte zwingend ein abgeschirmtes Kabel (z.B. CAT6/CAT7) verwendet werden.

### 🏠 Inneneinheit (Sichere Zone)
Hier befindet sich das "Gehirn" der Anlage.
* **Microcontroller:** ESP32 DevKit (38-Pin Version, ESP-WROOM-32).
* **Stromversorgung:** HLK-5M05 AC-DC Wandler (230V auf 5V / 5W). *Achtung: Arbeiten an 230V dürfen nur von Fachpersonal durchgeführt werden!*
* **Level Shifting:** SN74AHCT125N Quad-Bus-Buffer (wandelt 3.3V Signale des ESP32 sicher auf 5V für Peripherie).
* **Pufferung:** Elektrolytkondensatoren (2x 1000µF, 470µF, 220µF) glätten die Spannungsversorgung, insbesondere bei Lastspitzen des Fingerprint-Sensors.
* **Schnittstellen:** RJ45 Buchsen zur Verbindung mit der Außeneinheit. Eine Stiftleiste für zukünftige I2C-Erweiterungen (z.B. OLED-Display) ist vorgesehen.

### 🚪 Außeneinheit (Benutzerschnittstelle)
Diese Einheit wird außen an der Haustür montiert.
* **Biometrie:** R503 Kapazitiver Fingerabdrucksensor (UART-Kommunikation).
* **RFID/NFC:** PN532 Modul im SPI-Modus.
* **Umwelt:** DHT22 Sensor für Außentemperatur und Luftfeuchtigkeit.
* **Interaktion:** Zwei Hardware-Taster ("Haustür Button 1 & 2") und ein Türkontakt-Sensor.
* **Feedback:** Aktiver 5V Summer (Buzzer), angetrieben durch einen BC567 NPN Transistor. Visuelles Feedback über den LED-Ring des R503 Sensors.

---

## 🛠️ Die Schritt-für-Schritt Anleitung

Jeder kann dieses System bauen. Folge einfach stur diesen Schritten:

### Schritt 1: Hardware aufbauen
1. Bestelle die Platine anhand der Gerber-Dateien im beiliegenden ZIP-Archiv `Gerber_haustür-gate_PCB_haustür-gate_2026-01-25.zip`.
2. Löte die Platinen gemäß den Schaltplänen und bestücke die Komponenten.
3. **Verkabelung:** Verbinde Innen- und Außeneinheit zwingend **vor** dem Einschalten mit den RJ45-Kabeln (max. 30cm, geschirmt).

### Schritt 2: ESPHome vorbereiten
Aus Sicherheitsgründen nutzt dieses Projekt ausgelagerte Passwörter.
1. Erstelle im gleichen Verzeichnis wie die `insane-haustür-gate.yaml` eine Datei namens `secrets.yaml`.
2. Füge exakt diese Struktur mit deinen eigenen, echten Daten ein:
   ```yaml
   wifi_ssid: "DeinWlanName"
   wifi_password: "DeinWlanPasswort"
   wifi_ap_password: "EinSicheresFallbackPasswort"
   api_encryption_key: "GeneriereEinenBase64Key"
   ota_password: "DeinOTAPasswort"
   master_tag_id: '"DE-AD-BE-EF"' # WICHTIG: Die UID deines Master-Tags für den Lernmodus
   ```
   > **💡 Tipp zur Master-Tag ID:** Wenn du die ID noch nicht kennst, flashe das Gerät zuerst ohne Master-Tag, halte einen Tag an den Leser und suche im ESPHome-Log nach der Zeile `Zugriff verweigert: XX-XX-XX-XX`. Trage diese ID dann in die Secrets ein und flashe erneut.
3. Lade die Datei `insane-haustür-gate.yaml` in dein ESPHome-Dashboard hoch.
4. Schließe den **ESP32** per USB an deinen Rechner an und flashe ihn das allererste Mal ganz normal über das Kabel.
5. **Integration in Home Assistant:** Sobald der ESP32 im WLAN ist, taucht er unter "Geräte & Dienste" in Home Assistant auf. Konfiguriere ihn dort.
6. **Erster Test:** Gib einen Namen im Textfeld ein und lerne deinen ersten Finger oder Tag an!

---

## ⚙️ Home Assistant & Software-Funktionen

Nachdem das ESPHome-Gerät in Home Assistant integriert wurde, stehen zahlreiche Entitäten zur Verfügung:

### 1. Benutzerverwaltung & Namen
Ein einzigartiges Feature ist die direkte Benennung von Tags und Fingern.
1. Suche das Textfeld **"Name für neuen Zugang"** im Home Assistant Dashboard.
2. Tippe den Namen der Person ein (z.B. "Max Mustermann").
3. Führe anschließend den Anlernvorgang für NFC oder Fingerabdruck durch (siehe unten).
4. Das System verknüpft die ID mit dem Namen. Das Textfeld leert sich danach automatisch.
5. Öffnet Max nun die Tür, ändert sich der Sensor **"Letzter Zugang"** auf "Max Mustermann".

### 2. NFC / RFID Tags anlernen (Whitelist)
Das System nutzt eine lokale Whitelist, die als langer String im ESP32 gespeichert ist. Es gibt zwei Wege, Tags anzulernen:

* **Methode A (Über Home Assistant):**
  1. Optional: Namen in das Feld "Name für neuen Zugang" eingeben.
  2. Drücke den Button **"Nächsten Tag anlernen"**.
  3. Der Sensor "Lernmodus Status (NFC)" wechselt auf "Aktiv - Karte auflegen...".
  4. Halte den neuen Tag an den Leser. Der Tag wird gespeichert (Buzzer piept zur Bestätigung).
* **Methode B (Über das Master-Tag):**
  1. Du musst die UID eines speziellen "Master-Tags" in der `secrets.yaml` definieren.
  2. Halte das Master-Tag an den Leser. Das System wechselt in den Lernmodus (visuelles/akustisches Feedback).
  3. Halte nun neue Tags an den Leser, um diese anzulernen.

Um **alle** Tags zu löschen, drücke in Home Assistant auf **"Whitelist komplett löschen"**. Einzelne Tags können derzeit nur durch komplettes Löschen und Neuanlernen der restlichen Tags entfernt werden.

### 3. Fingerabdrücke anlernen
Der R503 Sensor verwaltet bis zu 200 Fingerabdrücke. Die Verwaltung der IDs übernimmt der ESP32 automatisch (`next_finger_id`).
1. Optional: Namen in das Feld "Name für neuen Zugang" eingeben.
2. Drücke den Button **"Neuen Finger anlernen (Auto-ID)"**.
3. Beobachte den Sensor **"Fingerprint Status"**. Er fordert dich auf, den Finger aufzulegen.
4. Lege den Finger **zweimal** auf den Sensor (warte auf das Blinken/Piepen dazwischen).
5. Bei Erfolg meldet der Status "Erfolgreich!" und die ID wird mit dem Namen verknüpft.

### 4. Status-Feedback (LED & Buzzer)
Der R503 Sensor verfügt über einen RGB-LED Ring (Aura LED), der zusammen mit dem Buzzer Feedback gibt:
* **Dauerhaft Blau:** System ist bereit (Idle).
* **Grün blinkend + kurzer Piepton:** Zugriff gewährt (Finger/Tag erkannt) oder Aktion erfolgreich.
* **Rot blinkend + langer Piepton:** Zugriff verweigert (unbekannter Tag/Finger) oder Fehler beim Anlernen.
* **Rot pulsierend:** Das System hat die WLAN-Verbindung verloren. (Türöffnung funktioniert weiterhin!).

### 5. Weitere Sensoren & Buttons
* **Haustürzugang (Lock-Entität):** Schaltet für 2 Sekunden auf `true`, wenn ein gültiger Finger oder Tag präsentiert wird. Diesen Sensor nutzt du in Home Assistant als Auslöser (Trigger), um deine tatsächliche Türöffner-Automation (Nuki, Relais etc.) zu starten.
* **Haustür Button 1 & 2:** Hardware-Taster an der Außeneinheit. Lösen in Home Assistant ein Event aus (z.B. als smarte Türklingel nutzbar). Beim Drücken ertönt ein kurzer Bestätigungston am Gate.
* **Temperatur & Luftfeuchtigkeit:** Liefert Umweltdaten des DHT22. Über die numerischen Felder "Temperatur Offset" und "Luftfeuchtigkeit Offset" können Ungenauigkeiten durch das Gehäuse direkt in Home Assistant korrigiert werden.

---

## 🔌 Technische Spezifikationen (Pin-Belegung)

Diese Belegung ist in der YAML fest kodiert. Wenn du eigene PCBs entwirfst, halte dich an dieses Mapping.

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

## ❓ Häufige Fragen & Troubleshooting

* **Der Fingerprint-Sensor leuchtet nicht / wird nicht erkannt.**
  * Prüfe die Verkabelung (RX/TX vertauscht?).
  * Stelle sicher, dass das Netzteil genug Strom liefert. Der Sensor zieht beim Scannen kurzzeitig viel Strom (dafür sind die Kondensatoren da).
* **Der NFC-Leser funktioniert sporadisch nicht.**
  * Das SPI-Signal ist sehr empfindlich. Ist das RJ45-Kabel zu lang (>30cm)? Ist es ungeschirmt? Verwende hochwertigere Kabel.
* **Die `whitelist_str` Variable ist zu lang / Speicher voll.**
  * ESPHome Strings in globalen Variablen sind technisch auf den verfügbaren RAM/Flash begrenzt. Für ein normales Einfamilienhaus (20-30 Tags) ist dies völlig unproblematisch. Bei hunderten Tags sollte das Konzept überdacht werden.
* **Wie ändere ich einen falsch geschriebenen Namen?**
  * Derzeit musst du den entsprechenden Tag/Finger ignorieren oder die Whitelist/Finger löschen und neu anlegen. (Finger können im R503 nicht einzeln überschrieben werden, ohne ein externes Tool zu nutzen).

---

## ⚖️ Lizenz
Dieses komplette Projekt (Hardware und Software) steht unter der [CC BY-NC-SA 4.0 Lizenz](https://creativecommons.org/licenses/by-nc-sa/4.0/).
Das bedeutet: Nachbauen und Anpassen für private Zwecke ist ausdrücklich erwünscht, jede kommerzielle Nutzung oder der Verkauf sind strikt verboten!

---

## ☕ Support dieses Projekts
Wenn dir das System gefällt und du meine Arbeit unterstützen möchtest, freue ich mich riesig über einen virtuellen Kaffee!

<a href="https://www.paypal.me/babeinlovexd">
  <img src="https://img.shields.io/badge/Donate-PayPal-blue.svg?style=for-the-badge&logo=paypal" alt="Donate mit PayPal">
</a>

---

## 👨‍💻 Entwickelt von

| [<img src="https://avatars.githubusercontent.com/u/43302033?v=4" width="100"><br><sub>**Christopher**</sub>](https://github.com/babeinlovexd) |
| :---: |

---
