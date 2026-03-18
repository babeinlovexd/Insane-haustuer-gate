# 🛠️ Troubleshooting & Statuscodes

Hier finden Sie Lösungen für häufige Probleme, die bei der Installation oder im Betrieb des **Insane Haustür Gate** auftreten können, sowie eine Übersicht über die Bedeutung der verschiedenen LED- und Buzzer-Signale.

---

## 🚦 Status-Feedback (LED & Buzzer)

Der R503 Sensor verfügt über einen RGB-LED Ring (Aura LED), der zusammen mit dem Buzzer wichtiges Feedback gibt:

*   **Dauerhaft Blau:** System ist bereit (Idle).
*   **Grün blinkend + kurzer Piepton:** Zugriff gewährt (Finger/Tag erkannt) oder Aktion erfolgreich (z.B. Tag angelernt).
*   **Rot blinkend + langer Piepton:** Zugriff verweigert (unbekannter Tag/Finger) oder Fehler beim Anlernen.
*   **Rot pulsierend:** Das System hat die WLAN-Verbindung verloren. (Die Türöffnung funktioniert weiterhin autonom!).

---

## ❓ Häufige Fragen & Troubleshooting

### Der Fingerprint-Sensor leuchtet nicht / wird nicht erkannt.
*   **Verkabelung prüfen:** Wurden RX und TX vertauscht (RX an TX, TX an RX)?
*   **Stromversorgung:** Stellen Sie sicher, dass das Netzteil (HLK-5M05) genug Strom liefert. Der Sensor zieht beim Scannen kurzzeitig viel Strom, weshalb die Stützkondensatoren (2x 1000µF, 470µF, 220µF) essenziell sind.

### Der NFC-Leser funktioniert sporadisch nicht.
*   Das SPI-Signal ist sehr empfindlich gegenüber Störungen.
*   **Kabellänge:** Ist das RJ45-Kabel zu lang (>30cm)?
*   **Schirmung:** Ist das Kabel ungeschirmt? Verwenden Sie zwingend hochwertigere, abgeschirmte Kabel (CAT6/CAT7).

### Die `whitelist_str` Variable ist zu lang / Speicher voll.
*   ESPHome Strings in globalen Variablen sind technisch auf den verfügbaren RAM/Flash begrenzt.
*   Für ein normales Einfamilienhaus (20-30 Tags) ist dies völlig unproblematisch. Bei hunderten Tags sollte das Konzept (oder die Hardware) überdacht werden.

### Wie ändere ich einen falsch geschriebenen Namen?
*   Derzeit müssen Sie den entsprechenden Tag/Finger ignorieren oder die Whitelist/Finger löschen und neu anlegen. (Finger können im R503 nicht einzeln überschrieben werden, ohne ein externes Tool zu nutzen).

### Kann ich das System hacken, wenn ich das Patchkabel abknipse?
*   Theoretisch sind **Hardware-Level Replay Attacks** (UART/SPI über das Kabel abgreifen) denkbar. Im Rahmen dieses Projekts wird dies jedoch **nicht** als Hauptrisiko betrachtet. Durch das Split-Design und ein maximal 30cm kurzes, abgeschirmtes Kabel (das z.B. durch die Wand oder den Türrahmen geführt wird) ist ein physischer Angriff in der Praxis meistens nur unter erheblicher Zerstörung der Haustür möglich.

---

### Weiterführende Hilfe

Wenn Sie ein Problem haben, das hier nicht gelistet ist:
1.  Prüfen Sie die ESPHome-Logs in Home Assistant oder über die Kommandozeile.
2.  Stellen Sie sicher, dass Sie die aktuellste Version der `insane-haustür-gate.yaml` verwenden.
3.  Erstellen Sie ein Issue im GitHub-Repository.