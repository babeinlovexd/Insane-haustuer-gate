# 💻 Software-Installation & Flashen

Diese Anleitung führt Sie durch die Schritte zur Konfiguration und zum Flashen des **Insane Haustür Gate**. Das System basiert auf [ESPHome](https://esphome.io/) und lässt sich nahtlos in Home Assistant integrieren.

## 1️⃣ Vorbereitungen: `secrets.yaml`

Um Ihre sensiblen Daten wie WLAN-Zugänge, Passwörter und Verschlüsselungskeys sicher zu hinterlegen, müssen Sie eine Datei namens `secrets.yaml` erstellen.

Erstellen Sie die Datei im gleichen Verzeichnis wie Ihre `insane-haustür-gate.yaml` und füllen Sie diese mit Ihren Daten:

```yaml
wifi_ssid: "IhrWlanName"
wifi_password: "IhrWlanPasswort"
wifi_ap_password: "EinSicheresFallbackPasswort"
api_encryption_key: "GenerierenSieEinenBase64Key"
ota_password: "IhrOTAPasswort"
master_tag_id: '"DE-AD-BE-EF"' # WICHTIG: Die UID Ihres Master-Tags für den Lernmodus
```

**Tipp zur Master-Tag ID:**
Wenn Sie die ID noch nicht kennen, lassen Sie den Eintrag zunächst leer oder nutzen Sie einen Platzhalter. Flashen Sie das Gerät, halten Sie dann den Tag an den Leser und suchen Sie im ESPHome-Log nach der Zeile `Zugriff verweigert: XX-XX-XX-XX`. Tragen Sie diese ID dann in die `secrets.yaml` ein und flashen Sie erneut.

---

## 2️⃣ Kompilieren & Erst-Flashen

Um das Gerät zum ersten Mal zu betreiben, müssen Sie die Firmware über USB flashen.

1.  Öffnen Sie das ESPHome Dashboard.
2.  Laden Sie die Konfigurationsdatei `insane-haustür-gate.yaml` und die neu erstellte `secrets.yaml` in Ihr Projektverzeichnis.
3.  Verbinden Sie den ESP32 (die **Inneneinheit**) über ein USB-Kabel mit Ihrem Computer/Server.
4.  Wählen Sie in ESPHome `Install` -> `Plug into this computer` und flashen Sie den Microcontroller.

*(Achtung: Achten Sie darauf, dass Innen- und Außeneinheit korrekt verbunden sind, bevor Sie das System über das Netzteil in Betrieb nehmen!)*

---

## 3️⃣ Integration in Home Assistant

Sobald der ESP32 geflasht ist und sich in Ihrem WLAN befindet, können Sie das Gerät in Home Assistant integrieren:

1.  Gehen Sie in Home Assistant zu **Einstellungen -> Geräte & Dienste**.
2.  Das Gerät "Haustür Gate" sollte dort unter den erkannten Geräten ("Entdeckt") auftauchen.
3.  Klicken Sie auf **Konfigurieren** und geben Sie bei Aufforderung den `api_encryption_key` aus Ihrer `secrets.yaml` ein.

---

## 4️⃣ Erster Test & Inbetriebnahme

Nach der erfolgreichen Integration stehen Ihnen alle Entitäten zur Verfügung.

1.  Das Gerät sollte auf dem ESPHome Dashboard als "Online" markiert sein.
2.  Sie können nun in Home Assistant das Dashboard öffnen, auf dem sich die Bedienelemente befinden.
3.  Geben Sie einen Namen im Textfeld ("Name für neuen Zugang") ein und lernen Sie Ihren ersten Finger oder Tag an (Siehe [Benutzerverwaltung](Benutzerverwaltung.md)).

### 📡 OTA Updates

Für zukünftige Änderungen an der Konfiguration können Sie das Gerät bequem "Over-The-Air" (OTA) updaten. Nutzen Sie dafür im ESPHome Dashboard die Option `Install` -> `Wirelessly`.
