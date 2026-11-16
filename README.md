<p align="center">
  <a href="https://github.com/babeinlovexd"><img src="https://raw.githubusercontent.com/babeinlovexd/babeinlovexd/main/bxd-github-header.svg" width="520" alt="BxD / BabeinlovexD"></a>
</p>

# Insane Haustür Gate

[Fragen & Fehler](https://github.com/babeinlovexd/Insane-haustuer-gate/issues) · [Releases](https://github.com/babeinlovexd/Insane-haustuer-gate/releases)

ESPHome-Projekt zur **Zugangserkennung mit Fingerabdruck und NFC** auf einem ESP32. Ein GROW R503, ein PN532 und lokale Benutzerdaten liefern ein Zugangssignal für eigene Home-Assistant-Automationen.

## Funktionen

- Lokale NFC-Whitelist und Fingerabdruckverwaltung mit Namenszuordnung.
- Anlernen über Home Assistant; NFC-Lernmodus zusätzlich über ein Master-Tag.
- Statusmeldungen, R503-LED-Ring und Summer als Rückmeldung.
- Türkontakt, zwei Taster sowie DHT22-Temperatur und Luftfeuchtigkeit.
- Getrennte Innen- und Außeneinheit mit RJ45-Verbindungen.

**Türöffnung:** Die mitgelieferte YAML enthält den binären Sensor **Haustürzugang**, aber keinen lokalen Relais-/Türöffner-Ausgang. Die tatsächliche Öffnung muss über eine passende Home-Assistant-Automation und einen Türöffner erfolgen. Die lokale Erkennung und Datenspeicherung allein ermöglichen daher noch keine vollständige Offline-Türöffnung. Eine lokale Ausgangssteuerung über herausgeführte Pins wäre eine eigene Hardware- und YAML-Erweiterung.

## Hardware & Unterlagen

| Teil | Referenzaufbau |
| :--- | :--- |
| Controller | ESP32 DevKit / ESP-WROOM-32, 38 Pins |
| Fingerabdruck | GROW R503 über UART |
| NFC | PN532 im SPI-Modus |
| Weitere Eingänge | DHT22, Türkontakt, zwei Taster |
| Summer-Treiber | **BC557-B, PNP** in der aktuellen V2-Bestückung |
| Versorgung | 5 V; im ursprünglichen Aufbau HLK-5M05-Netzteilmodul |

Die aktuelle Hardware ist **V2**. Passende Fertigungsunterlagen befinden sich in den [Releases](https://github.com/babeinlovexd/Insane-haustuer-gate/releases). Die ältere Gerber-ZIP im Repository-Stamm stammt aus Januar 2026 und darf nicht ungeprüft mit V2-Bestückung vermischt werden. Verwende zusammengehörige Platinen-, Schaltplan- und Bestückungsstände.

Die RJ45-Leitungen übertragen UART-/SPI- und Peripheriesignale, **kein Ethernet**. Der Referenzaufbau sieht kurze, geschirmte Verbindungen bis 30 cm vor. Arbeiten an einer netzspannungsführenden Versorgung gehören in fachkundige Hände.

## Installation

1. Den zur Hardware passenden Stand von [insane-haustür-gate.yaml](insane-haustür-gate.yaml) herunterladen und im ESPHome-Konfigurationsordner ablegen.
2. Daneben eine eigene `secrets.yaml` anlegen:

```yaml
api_encryption_key: "DEIN_API_KEY"
ota_password: "DEIN_OTA_PASSWORT"
wifi_ssid: "DEIN_WLAN_NAME"
wifi_password: "DEIN_WLAN_PASSWORT"
ap_password: "DEIN_FALLBACK_PASSWORT"
```

Zusätzlich wird der Master-Tag benötigt. Der Wert wird als C++-Stringliteral in eine Initialisierung eingesetzt:

```yaml
master_tag_id: '"DE-AD-BE-EF"'
```

`DE-AD-BE-EF` durch die eigene UID ersetzen. Der Schlüssel für das Fallback-WLAN lautet **`ap_password`**, nicht `wifi_ap_password`.

3. Konfiguration in ESPHome prüfen und kompilieren. Das erste Flashen des ESP32 per USB durchführen.
4. Das Gerät in Home Assistant über die ESPHome-Integration verbinden.
5. Einen Zugang anlernen und die Rückmeldungen prüfen. Erst danach die eigene Türöffner-Automation mit dem Zugangssignal verbinden und separat testen.

## Bedienung

### NFC und Namen

Im Textfeld **Name für neuen Zugang** einen Namen eingeben und **Nächsten Tag anlernen** drücken. Danach den Tag auflegen. Alternativ aktiviert das konfigurierte Master-Tag den NFC-Lernmodus. Der Sensor **Letzter Zugang** zeigt den zugeordneten Namen an.

**Whitelist komplett löschen** entfernt alle NFC-Einträge. Eine komfortable Einzelbearbeitung der NFC-Whitelist ist in dieser Konfiguration nicht vorgesehen.

### Fingerabdrücke

Über **Neuen Finger anlernen (Auto-ID)** eine ID anlegen und den Aufforderungen des Statussensors folgen. Die Konfiguration begrenzt die automatische Vergabe auf 200 Finger-IDs. Bei einer kompletten Leerung der Fingerabdruckdatenbank wird die nächste ID zurückgesetzt.

### Zugangssignal und Rückmeldung

**Haustürzugang** ist ein `binary_sensor` mit `device_class: lock` – keine steuerbare `lock`-Entität. Er liefert bei Erkennung einen zeitlich begrenzten Zustand für Automationen. Grün signalisiert erfolgreiche Erkennung, Rot eine Ablehnung bzw. einen Fehler. Die beiden Taster und der Türkontakt werden separat in Home Assistant bereitgestellt.

## Pin-Belegung

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

RX/TX sind aus Sicht des ESP32 angegeben. Die Leitungen zum Sensor müssen entsprechend gekreuzt werden.

## Fehlersuche

- **R503 antwortet nicht:** Versorgung, gemeinsame Masse, UART-Belegung und Baudrate prüfen.
- **NFC sporadisch:** SPI-Verbindung, Leitungslänge und Abschirmung prüfen.
- **Erkennung klappt, Tür bleibt zu:** Zustand von **Haustürzugang**, Home-Assistant-Automation und eigentlichen Türöffner getrennt prüfen.
- **Summer verhält sich falsch:** V2-Bestückung **BC557-B**, Anschlussbelegung und invertierte Ansteuerung mit dem Schaltplan abgleichen.
- **Speicher oder Namen:** Für eine Neuorganisation die vorhandenen Lösch-/Anlernfunktionen verwenden und vor dem Löschen die benötigten Zuordnungen notieren.

## Lizenz

[CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/). Es gilt die bestehende Projektlizenz. Drittanbieter-Komponenten behalten ihre jeweiligen Lizenzen.

---

## Builds und Releases

Version: **2.2.0** ([VERSION](VERSION)). [GitHub Actions](.github/workflows/release.yml) prüft Pushes und Pull Requests. Ein Tag passend zu `v` + `VERSION` veröffentlicht das Release erst nach erfolgreichen Jobs; manuelle Läufe liefern Build-Artefakte. [Downloads](https://github.com/babeinlovexd/Insane-haustuer-gate/releases).

ESPHome-CI verwendet Platzhalter-Zugangsdaten und flasht keine Geräte. Vor dem Einsatz eigene Secrets und Pinbelegung prüfen.

## Mitmachen

Fragen, Fehlerberichte und Beiträge sind willkommen. Nenne Version, Aufbau und Schritte zum Nachstellen im [Issue](https://github.com/babeinlovexd/Insane-haustuer-gate/issues). Größere Änderungen besprechen wir am besten vor einem Pull Request.

Dieses Hobbyprojekt unterstützen: [PayPal](https://www.paypal.me/babeinlovexd).

[BxD / BabeinlovexD](https://github.com/babeinlovexd) · [Website](https://babeinlovexd.de/)
