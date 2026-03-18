<div align="center">
  <h1>Insane Haustür Gate</h1>
  <p><i>Smarte Zutrittskontrolle mit biometrischer und NFC-Validierung</i></p>
</div>

<hr>

<h3>Projektbeschreibung</h3>
<p>
  Das Haustür Gate ist eine maßgeschneiderte Sicherheitslösung für moderne Smart Homes, die eine zuverlässige Brücke zwischen physischer Sicherheit und digitaler Steuerung schlägt. 
  Das System ermöglicht den schlüssellosen Zugang mittels Fingerabdruck oder NFC-Chips und ist nativ in Home Assistant eingebunden, um maximale Flexibilität bei Automationen zu bieten. 
</p>
<p>
  Ein zentraler Aspekt des Designs ist die physische Trennung von Logik und Interaktion: Während die Außeneinheit lediglich als Terminal für Sensoren und Taster dient, verbleiben alle sicherheitsrelevanten Komponenten – einschließlich der Stromversorgung und des ESP32-Controllers – geschützt in der Inneneinheit. 
  Diese Architektur verhindert effektiv Manipulationsversuche von außen und gewährleistet durch die Verbindung via RJ45-Patchkabel eine stabile Signalübertragung über längere Distanzen.
</p>

<h3>Systemarchitektur</h3>
<p>
  Das System basiert auf einer Split-Hardware-Architektur, um Funktionalität und Sicherheit im Außenbereich zu gewährleisten:
</p>

<ul>
  <li>
    <b>Inneneinheit:</b> Fungiert als Steuerzentrale mit einem ESP32 DevKit, dem AC-DC Wandler (HLK-5M05) und dem Logikpegelwandler. 
    Für zukünftige Erweiterungen ist eine <b>Stiftleiste für Upgrades</b> integriert, die den einfachen Anschluss von I2C-Komponenten wie OLED-Displays ermöglicht.
  </li>
  <li>
    <b>Außeneinheit:</b> Dient als Benutzerschnittstelle und beherbergt den biometrischen R503 Sensor, das PN532 NFC-Modul sowie Umwelt- und Interaktionselemente.
  </li>
  <li>
    <b>Konnektivität:</b> Beide Einheiten sind über <b>zwei Standard-RJ45-Patchkabel</b> miteinander verbunden, was eine einfache Installation und Wartung ermöglicht.
  </li>
</ul>

<h3>Stückliste (Bill of Materials)</h3>
<table width="100%">
  <thead>
    <tr>
      <th align="left">Komponente</th>
      <th align="left">Spezifikation</th>
      <th align="center">Menge</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>ESP32 DevKit</td>
      <td>38-Pin Version (ESP-WROOM-32)</td>
      <td align="center">1</td>
    </tr>
    <tr>
      <td>R503 Sensor</td>
      <td>Kapazitiver Fingerabdrucksensor (UART)</td>
      <td align="center">1</td>
    </tr>
    <tr>
      <td>PN532 Modul</td>
      <td>NFC/RFID Controller (SPI-Mode)</td>
      <td align="center">1</td>
    </tr>
    <tr>
      <td>DHT22</td>
      <td>Temperatur- & Luftfeuchtigkeitssensor</td>
      <td align="center">1</td>
    </tr>
    <tr>
      <td>HLK-5M05</td>
      <td>AC-DC Wandler (230V auf 5V / 5W)</td>
      <td align="center">1</td>
    </tr>
    <tr>
      <td>SN74AHCT125N</td>
      <td>Quad-Bus-Buffer (Logic Level Shifter 3.3V auf 5V)</td>
      <td align="center">1</td>
    </tr>
    <tr>
      <td>Buzzer</td>
      <td>Aktiver 5V Summer</td>
      <td align="center">1</td>
    </tr>
    <tr>
      <td>BC567</td>
      <td>NPN Transistor (Buzzer-Treiber)</td>
      <td align="center">1</td>
    </tr>
    <tr>
      <td><b>Elektrolytkondensator</b></td>
      <td><b>1000µF (Glättung & Pufferung)</b></td>
      <td align="center"><b>2</b></td>
    </tr>
    <tr>
      <td>Elektrolytkondensator</td>
      <td>470µF / 220µF</td>
      <td align="center">je 1</td>
    </tr>
    <tr>
      <td>Widerstand</td>
      <td>1kΩ (Basiswiderstand)</td>
      <td align="center">1</td>
    </tr>
    <tr>
      <td>RJ45 Buchsen</td>
      <td>Printmontage für Patchkabel-Link</td>
      <td align="center">4</td>
    </tr>
  </tbody>
</table>

<h3>Home Assistant & Software-Funktionen</h3>
<p>
  Dank der ESPHome-Integration bietet das System eine intuitive Verwaltung direkt über das Home Assistant Dashboard:
</p>

<ul>
  <li>
    <b>Biometrische Registrierung:</b> Über den Button "Neuen Finger anlernen" wird der Enrollment-Prozess gestartet. Das System verwaltet bis zu 200 Fingerabdrücke und vergibt IDs automatisch.
  </li>
  <li>
    <b>NFC-Management:</b> Unterstützt das Hinzufügen von UIDs über einen Lernmodus, der entweder per Software-Button oder durch ein dediziertes Master-Tag aktiviert wird.
  </li>
  <li>
    <b>Dynamische Namensspeicherung:</b> Über das Textfeld "Name für neuen Zugang" in Home Assistant kann der Name einer Person eingetippt werden. Drückt man anschließend auf "Anlernen" oder legt im NFC-Lernmodus ein Tag auf, wird der Name lokal auf dem Gate gespeichert und mit dem Tag/Fingerabdruck verknüpft. Sobald die Person später die Tür öffnet, wird der Name im Sensor "Letzter Zugang" angezeigt. Das Textfeld leert sich nach dem Speichern automatisch.
  </li>
  <li>
    <b>Benutzer-Interaktion:</b> Zwei programmierbare Buttons (Haustür Button 1 & 2) können für beliebige HA-Aktionen wie Klingelfunktionen oder Lichtsteuerung genutzt werden.
  </li>
  <li>
    <b>Status-Feedback:</b> Akustische Signale über den Buzzer sowie visuelles Feedback über die Aura-LED des R503 Sensors informieren über den Erfolg oder Fehler eines Zutrittsversuchs.
  </li>
</ul>

<h3>Installation & Konfiguration</h3>
<p>
  Für die Inbetriebnahme sind folgende Schritte erforderlich:
</p>

<ul>
  <li>
    <b>Hardware-Setup:</b> Verbinden Sie die Innen- und Außeneinheit über die RJ45-Schnittstellen, bevor Sie die Netzspannung (230V) an das HLK-5M05 Modul anlegen.
  </li>
  <li>
    <b>Master-Tag Konfiguration:</b> Um den administrativen Zugriff vor Ort zu ermöglichen, muss die UID Ihres persönlichen Master-Tags händisch in der Datei <code>haustur-gate.yaml</code> im Abschnitt <code>globals</code> hinterlegt werden:
    <br>
    <code style="background-color: rgba(0,0,0,0.05); padding: 2px 5px; border-radius: 3px;">initial_value: '"DE-AD-BE-EF"'</code>
  </li>
  <li>
    <b>Firmware-Flash:</b> Kompilieren Sie die Konfiguration in ESPHome und führen Sie den ersten Flash-Vorgang via USB durch. Zukünftige Updates können bequem über die OTA-Schnittstelle (Over-the-Air) eingespielt werden.
  </li>
  <li>
    <b>Home Assistant:</b> Nach dem erfolgreichen Booten wird das Gerät automatisch als neue Integration erkannt. Fügen Sie es hinzu, um die Entitäten für das Fingerabdruck-Management und die NFC-Whitelist zu erhalten.
  </li>
</ul>

<h3>Technische Spezifikationen (Pin-Belegung)</h3>
<table width="100%">
  <thead>
    <tr>
      <th align="left">Schnittstelle</th>
      <th align="left">GPIO Pins</th>
      <th align="left">Details</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Fingerprint (UART)</td>
      <td>16 (RX), 17 (TX), 33 (Sensing)</td>
      <td>57600 baud</td>
    </tr>
    <tr>
      <td>NFC Reader (SPI)</td>
      <td>18 (CLK), 19 (MISO), 23 (MOSI), 5 (CS)</td>
      <td>SPI Bus</td>
    </tr>
    <tr>
      <td>Umweltsensor</td>
      <td>4 (DHT22)</td>
      <td>Temperatur & Feuchtigkeit</td>
    </tr>
    <tr>
      <td>Signalgeber</td>
      <td>2 (Buzzer)</td>
      <td>PWM-gesteuert</td>
    </tr>
    <tr>
      <td>Eingänge</td>
      <td>14, 27 (Buttons), 32 (Kontakt)</td>
      <td>Input mit Pullup</td>
    </tr>
  </tbody>
</table>

<hr>

<p align="center">
  <i>Hinweis: Die zugehörigen Gerber-Dateien für die Fertigung der Platinen sind im Unterordner verfügbar.</i>
</p>
