# 🔒 Sicherheitskonzept

Das **Insane Haustür Gate** wurde mit dem Ziel entwickelt, eine Balance zwischen smartem Komfort und hoher physischer sowie datentechnischer Sicherheit zu schaffen.

---

## 🛡️ Das Split-Design

Die wichtigste Maßnahme zur Absicherung gegen physische Manipulation ist das sogenannte **Split-Design** (geteilte Architektur).

1.  **Inneneinheit (Sichere Zone):**
    *   Der eigentliche Microcontroller (ESP32) befindet sich im geschützten Inneren des Hauses.
    *   Die gesamte Logik zur Überprüfung der Berechtigung, die Speicherung der Zugangsdaten und die Steuerung von Relais/Türöffnern passiert hier.
    *   Dies bedeutet, dass ein potenzieller Angreifer, der die Außeneinheit manipuliert oder zerstört, nicht an die Steuerungselektronik oder den Türöffner gelangt.

2.  **Außeneinheit (Benutzerschnittstelle):**
    *   Die Außeneinheit an der Tür ist "dumm". Sie besteht lediglich aus Sensoren (NFC, Fingerprint), Tastern und LEDs.
    *   Sie empfängt Eingaben und sendet sie an die Inneneinheit, hat aber keine Entscheidungsgewalt oder Kontrolle über kritische Vorgänge.

### 🔌 Physische Sicherheit bei der Verkabelung

Die Verbindung zwischen Innen- und Außeneinheit erfolgt über ein Standard-RJ45-Patchkabel, das die UART- und SPI-Signale überträgt.

**Hardware-Level Replay Attacks:**
Es ist theoretisch möglich, über das UART- oder SPI-Kabel Kommunikationsdaten abzugreifen und aufzuzeichnen (Replay-Angriff), um später einen autorisierten Zugang vorzutäuschen. Im Rahmen dieses Projekts wird dieses Risiko bewusst **nicht** adressiert, da die Angriffsfläche durch die folgenden Punkte stark minimiert ist:
*   Die Kabellänge ist auf **maximal 30 cm** begrenzt.
*   Es wird zwingend ein **abgeschirmtes Kabel** (z.B. CAT6/CAT7) verwendet.
*   Der Zugang zum Kabel an der Außenseite ist für Angreifer meist nur unter erheblicher Zerstörung der Fassade/des Türrahmens möglich.

---

## 💾 Lokale Datenhaltung & Autonomie

Ein weiterer wichtiger Aspekt des Sicherheits- und Stabilitätskonzepts ist die völlige Autonomie des ESP32.

*   **Offline-Fähigkeit:** Die Whitelist (NFC-Tags) und Fingerabdruck-IDs werden im lokalen Flash-Speicher des ESP32 (`restore_value: yes`) vorgehalten. Selbst wenn das WLAN ausfällt oder der Home Assistant-Server nicht erreichbar ist, funktioniert die Türöffnung problemlos weiter.
*   **Datenschutz:** Die Zuweisung von Namen zu IDs ("Max Mustermann" -> "Tag-ID X") wird lokal gespeichert. Es werden keine Zugangsdaten in einer Cloud hinterlegt. Die Home Assistant Instanz dient primär als bequeme UI für die Verwaltung.

---

## 🔄 API-Verschlüsselung & OTA Updates

*   **API-Kommunikation:** Die Verbindung zwischen dem ESP32 und Home Assistant erfolgt standardmäßig verschlüsselt, wenn in der `secrets.yaml` ein `api_encryption_key` definiert ist. Dies verhindert Man-in-the-Middle-Angriffe im lokalen Netzwerk.
*   **OTA (Over-The-Air) Updates:** Firmware-Updates können sicher über das WLAN aufgespielt werden, abgesichert durch das in der `secrets.yaml` hinterlegte `ota_password`.