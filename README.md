<div align="center">
  <h1>🚪 Insane Haustür Gate</h1>
  <p><i>Smarte Zutrittskontrolle mit biometrischer und NFC-Validierung für Home Assistant (via ESPHome)</i></p>
</div>

<hr>

## 📋 Projektbeschreibung

Das **Haustür Gate** ist eine vollumfängliche, smarte Zutrittslösung für Home Assistant. Es kombiniert einen biometrischen Fingerabdrucksensor (R503) mit einem NFC-Lesegerät (PN532), um eine sichere, schlüssellose Türöffnung zu ermöglichen. Die Architektur ist für maximale Sicherheit und Zuverlässigkeit ausgelegt.

**Kernmerkmale des Systems:**
*   **Autonome Funktionalität:** Der ESP32 verwaltet Zugangsdaten (NFC-Whitelist und Fingerabdruck-IDs) lokal in seinem Flash-Speicher (via ESPHome `restore_value: yes`). Selbst bei einem Ausfall des WLANs oder von Home Assistant bleibt die Türöffnungsfunktion erhalten!
*   **Physische Sicherheit (Split-Design):** Das System ist in zwei Einheiten aufgeteilt. Die sicherheitsrelevante Logik (ESP32, Stromversorgung, Relais/Türöffner-Ansteuerung) befindet sich in einer **geschützten Inneneinheit**. Die **Außeneinheit** dient nur als Schnittstelle (Sensoren, Taster, LEDs).
*   **Datenschutz & Komfort:** Namen von Benutzern können direkt über Home Assistant beim Anlernen eingegeben werden. Das System speichert diese Zuordnung (UID/Finger-ID zu Name) lokal und meldet bei Zutritt den Namen des Benutzers an Home Assistant zurück ("Letzter Zugang").

---

## 🏗️ Systemarchitektur & Hardware

Die Trennung von Innen- und Außeneinheit wird durch eine einfache Verbindung via Standard-RJ45-Patchkabel realisiert. Da hierüber sensible UART/SPI-Signale übertragen werden, ist die Kabellänge auf **maximal 30 cm** begrenzt und es sollte zwingend ein abgeschirmtes Kabel (z.B. CAT6/CAT7) verwendet werden.

### 🏠 Inneneinheit (Sichere Zone)
Hier befindet sich das "Gehirn" der Anlage.
*   **Microcontroller:** ESP32 DevKit (38-Pin Version, ESP-WROOM-32).
*   **Stromversorgung:** HLK-5M05 AC-DC Wandler (230V auf 5V / 5W). *Achtung: Arbeiten an 230V dürfen nur von Fachpersonal durchgeführt werden!*
*   **Level Shifting:** SN74AHCT125N Quad-Bus-Buffer (wandelt 3.3V Signale des ESP32 sicher auf 5V für Peripherie).
*   **Pufferung:** Elektrolytkondensatoren (2x 1000µF, 470µF, 220µF) glätten die Spannungsversorgung, insbesondere bei Lastspitzen des Fingerprint-Sensors.
*   **Schnittstellen:** RJ45 Buchsen zur Verbindung mit der Außeneinheit. Eine Stiftleiste für zukünftige I2C-Erweiterungen (z.B. OLED-Display) ist vorgesehen.

### 🚪 Außeneinheit (Benutzerschnittstelle)
Diese Einheit wird außen an der Haustür montiert.
*   **Biometrie:** R503 Kapazitiver Fingerabdrucksensor (UART-Kommunikation).
*   **RFID/NFC:** PN532 Modul im SPI-Modus.
*   **Umwelt:** DHT22 Sensor für Außentemperatur und Luftfeuchtigkeit.
*   **Interaktion:** Zwei Hardware-Taster ("Haustür Button 1 & 2") und ein Türkontakt-Sensor.
*   **Feedback:** Aktiver 5V Summer (Buzzer), angetrieben durch einen BC567 NPN Transistor. Visuelles Feedback über den LED-Ring des R503 Sensors.

---

## ⚙️ Home Assistant & Software-Funktionen (Mini-Doku)

Nachdem das ESPHome-Gerät in Home Assistant integriert wurde, stehen zahlreiche Entitäten zur Verfügung. Hier wird im Detail erklärt, wie das System bedient wird:

### 1. Benutzerverwaltung & Namen
Ein einzigartiges Feature ist die direkte Benennung von Tags und Fingern.
1.  Suchen Sie das Textfeld **"Name für neuen Zugang"** im Home Assistant Dashboard.
2.  Tippen Sie den Namen der Person ein (z.B. "Max Mustermann").
3.  Führen Sie anschließend den Anlernvorgang für NFC oder Fingerabdruck durch (siehe unten).
4.  Das System verknüpft die ID mit dem Namen. Das Textfeld leert sich danach automatisch.
5.  Öffnet Max nun die Tür, ändert sich der Sensor **"Letzter Zugang"** auf "Max Mustermann".

### 2. NFC / RFID Tags anlernen (Whitelist)
Das System nutzt eine lokale Whitelist, die als langer String im ESP32 gespeichert ist. Es gibt zwei Wege, Tags anzulernen:

*   **Methode A (Über Home Assistant):**
    1.  Optional: Namen in das Feld "Name für neuen Zugang" eingeben.
    2.  Drücken Sie den Button **"Nächsten Tag anlernen"**.
    3.  Der Sensor "Lernmodus Status (NFC)" wechselt auf "Aktiv - Karte auflegen...".
    4.  Halten Sie den neuen Tag an den Leser. Der Tag wird gespeichert (Buzzer piept zur Bestätigung).
*   **Methode B (Über das Master-Tag):**
    1.  Sie müssen die UID eines speziellen "Master-Tags" in der `secrets.yaml` definieren.
    2.  Halten Sie das Master-Tag an den Leser. Das System wechselt in den Lernmodus (visuelles/akustisches Feedback).
    3.  Halten Sie nun neue Tags an den Leser, um diese anzulernen.

Um **alle** Tags zu löschen, drücken Sie in Home Assistant auf **"Whitelist komplett löschen"**. Einzelne Tags können derzeit nur durch komplettes Löschen und Neuanlernen der restlichen Tags entfernt werden.

### 3. Fingerabdrücke anlernen
Der R503 Sensor verwaltet bis zu 200 Fingerabdrücke. Die Verwaltung der IDs übernimmt der ESP32 automatisch (`next_finger_id`).
1.  Optional: Namen in das Feld "Name für neuen Zugang" eingeben.
2.  Drücken Sie den Button **"Neuen Finger anlernen (Auto-ID)"**.
3.  Beobachten Sie den Sensor **"Fingerprint Status"**. Er fordert Sie auf, den Finger aufzulegen.
4.  Legen Sie den Finger **zweimal** auf den Sensor (warten Sie auf das Blinken/Piepen dazwischen).
5.  Bei Erfolg meldet der Status "Erfolgreich!" und die ID wird mit dem Namen verknüpft.

### 4. Status-Feedback (LED & Buzzer)
Der R503 Sensor verfügt über einen RGB-LED Ring (Aura LED), der zusammen mit dem Buzzer Feedback gibt:
*   **Dauerhaft Blau:** System ist bereit (Idle).
*   **Grün blinkend + kurzer Piepton:** Zugriff gewährt (Finger/Tag erkannt) oder Aktion erfolgreich.
*   **Rot blinkend + langer Piepton:** Zugriff verweigert (unbekannter Tag/Finger) oder Fehler beim Anlernen.
*   **Rot pulsierend:** Das System hat die WLAN-Verbindung verloren. (Türöffnung funktioniert weiterhin!).

### 5. Weitere Sensoren & Buttons
*   **Haustürzugang (Lock-Entität):** Schaltet für 2 Sekunden auf `true`, wenn ein gültiger Finger oder Tag präsentiert wird. Diesen Sensor nutzen Sie in Home Assistant als Auslöser (Trigger), um Ihre tatsächliche Türöffner-Automation (Nuki, Relais etc.) zu starten.
*   **Haustür Button 1 & 2:** Hardware-Taster an der Außeneinheit. Lösen in Home Assistant ein Event aus (z.B. als smarte Türklingel nutzbar). Beim Drücken ertönt ein kurzer Bestätigungston am Gate.
*   **Temperatur & Luftfeuchtigkeit:** Liefert Umweltdaten des DHT22. Über die numerischen Felder "Temperatur Offset" und "Luftfeuchtigkeit Offset" können Ungenauigkeiten durch das Gehäuse direkt in Home Assistant korrigiert werden.

---

## 🚀 Installation & Inbetriebnahme (Schritt-für-Schritt)

1.  **Hardware-Aufbau:** Löten Sie die Platinen gemäß den Schaltplänen und bestücken Sie die Komponenten. *Gerber-Dateien für die PCB-Fertigung liegen im ZIP-Archiv bei.*
2.  **Verkabelung:** Verbinden Sie Innen- und Außeneinheit zwingend **vor** dem Einschalten mit den RJ45-Kabeln (max. 30cm, geschirmt).
3.  **Secrets konfigurieren:** Erstellen Sie im gleichen Verzeichnis wie die `insane-haustür-gate.yaml` eine Datei namens `secrets.yaml` und füllen Sie diese mit Ihren Daten:
    ```yaml
    wifi_ssid: "IhrWlanName"
    wifi_password: "IhrWlanPasswort"
    wifi_ap_password: "EinSicheresFallbackPasswort"
    api_encryption_key: "GenerierenSieEinenBase64Key"
    ota_password: "IhrOTAPasswort"
    master_tag_id: '"DE-AD-BE-EF"' # WICHTIG: Die UID Ihres Master-Tags für den Lernmodus
    ```
    *Tipp zur Master-Tag ID:* Wenn Sie die ID noch nicht kennen, flashen Sie das Gerät zuerst ohne Master-Tag, halten Sie einen Tag an den Leser und suchen Sie im ESPHome-Log nach der Zeile `Zugriff verweigert: XX-XX-XX-XX`. Tragen Sie diese ID dann in die Secrets ein und flashen Sie erneut.
4.  **Flashen:** Kompilieren Sie die `insane-haustür-gate.yaml` über das ESPHome Dashboard und flashen Sie den ESP32 das erste Mal per USB-Kabel.
5.  **Integration in Home Assistant:** Sobald der ESP32 im WLAN ist, taucht er unter "Geräte & Dienste" in Home Assistant auf. Konfigurieren Sie ihn dort.
6.  **Erster Test:** Geben Sie einen Namen im Textfeld ein und lernen Sie Ihren ersten Finger oder Tag an!

---

## 🔌 Technische Spezifikationen (Pin-Belegung)

Diese Belegung ist in der YAML fest kodiert. Wenn Sie eigene PCBs entwerfen, halten Sie sich an dieses Mapping.

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

*   **Der Fingerprint-Sensor leuchtet nicht / wird nicht erkannt.**
    *   Prüfen Sie die Verkabelung (RX/TX vertauscht?).
    *   Stellen Sie sicher, dass das Netzteil genug Strom liefert. Der Sensor zieht beim Scannen kurzzeitig viel Strom (dafür sind die Kondensatoren da).
*   **Der NFC-Leser funktioniert sporadisch nicht.**
    *   Das SPI-Signal ist sehr empfindlich. Ist das RJ45-Kabel zu lang (>30cm)? Ist es ungeschirmt? Verwenden Sie hochwertigere Kabel.
*   **Die `whitelist_str` Variable ist zu lang / Speicher voll.**
    *   ESPHome Strings in globalen Variablen sind technisch auf den verfügbaren RAM/Flash begrenzt. Für ein normales Einfamilienhaus (20-30 Tags) ist dies völlig unproblematisch. Bei hunderten Tags sollte das Konzept überdacht werden.
*   **Wie ändere ich einen falsch geschriebenen Namen?**
    *   Derzeit müssen Sie den entsprechenden Tag/Finger ignorieren oder die Whitelist/Finger löschen und neu anlegen. (Finger können im R503 nicht einzeln überschrieben werden, ohne ein externes Tool zu nutzen).

<p align="center">
  <i>💡 <b>Hinweis:</b> Die zugehörigen Gerber-Dateien für die Fertigung der Platinen sind im Unterordner als ZIP-Archiv verfügbar.</i>
</p>
