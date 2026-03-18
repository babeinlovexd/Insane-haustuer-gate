<div align="center">
  <h1>🚪 Insane Haustür Gate Wiki</h1>
  <p><i>Smarte Zutrittskontrolle mit biometrischer und NFC-Validierung für Home Assistant (via ESPHome)</i></p>
</div>

<hr>

Willkommen im offiziellen Wiki für das **Insane Haustür Gate**!

Das Projekt bietet eine sichere, schlüssellose Türöffnung durch die Kombination eines biometrischen Fingerabdrucksensors (R503) mit einem NFC-Lesegerät (PN532). Es ist so konzipiert, dass es vollständig in Home Assistant integriert wird, jedoch autonom weiterarbeitet, selbst wenn WLAN oder das Smart Home System ausfallen.

Dieses Wiki bietet detaillierte Informationen zum System, zur Hardware, Installation und zur Verwaltung.

## 📚 Inhaltsverzeichnis

*   [Hardware-Setup & Architektur](Hardware-Setup.md) - Details zu den verwendeten Bauteilen (Innen- und Außeneinheit), Platinen und Pin-Belegung.
*   [Software-Installation & Flashen](Software-Installation.md) - Anleitung zum Erstellen der `secrets.yaml` und zum Flashen mit ESPHome.
*   [Benutzerverwaltung & Berechtigungen](Benutzerverwaltung.md) - Wie man Fingerabdrücke und NFC-Tags anlernt, benennt und verwaltet.
*   [Sicherheitskonzept](Sicherheit.md) - Warum das Split-Design so sicher ist und wie lokale Daten verwaltet werden.
*   [Troubleshooting & Statuscodes](Troubleshooting.md) - Häufige Probleme, ihre Lösungen und die Bedeutung der LED-/Buzzer-Signale.

---

### 🌟 Kernmerkmale auf einen Blick

1.  **Autonome Funktionalität:** Der ESP32 verwaltet Zugangsdaten (NFC-Whitelist und Fingerabdruck-IDs) lokal in seinem Flash-Speicher (via ESPHome `restore_value: yes`).
2.  **Physische Sicherheit (Split-Design):** Das System ist in zwei Einheiten aufgeteilt. Die sicherheitsrelevante Logik befindet sich in einer geschützten Inneneinheit, während außen nur Sensoren und Taster platziert sind.
3.  **Datenschutz & Komfort:** Namen von Benutzern können direkt über Home Assistant beim Anlernen eingegeben werden. Das System speichert diese Zuordnung und meldet bei Zutritt den Namen des Benutzers zurück.
