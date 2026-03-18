# 👥 Benutzerverwaltung & Berechtigungen

Das **Insane Haustür Gate** zeichnet sich durch seine lokale, vollautomatische Benutzerverwaltung aus. Die Verwaltung (Anlernen, Zuweisen von Namen, Löschen) erfolgt bequem über Home Assistant.

---

## 🏷️ Benutzerverwaltung & Namen

Ein einzigartiges Feature ist die direkte Benennung von Tags und Fingern.

1.  Suchen Sie das Textfeld **"Name für neuen Zugang"** im Home Assistant Dashboard.
2.  Tippen Sie den Namen der Person ein (z.B. "Max Mustermann").
3.  Führen Sie anschließend den Anlernvorgang für NFC oder Fingerabdruck durch (siehe unten).
4.  Das System verknüpft die ID mit dem Namen. Das Textfeld leert sich danach automatisch.
5.  Öffnet Max nun die Tür, ändert sich der Sensor **"Letzter Zugang"** auf "Max Mustermann".

---

## 🔑 NFC / RFID Tags anlernen (Whitelist)

Das System nutzt eine lokale Whitelist, die als langer String im ESP32 gespeichert ist. Es gibt zwei Wege, Tags anzulernen:

### Methode A (Über Home Assistant):
1.  Optional: Namen in das Feld "Name für neuen Zugang" eingeben.
2.  Drücken Sie den Button **"Nächsten Tag anlernen"**.
3.  Der Sensor "Lernmodus Status (NFC)" wechselt auf "Aktiv - Karte auflegen...".
4.  Halten Sie den neuen Tag an den Leser. Der Tag wird gespeichert (Buzzer piept zur Bestätigung).

### Methode B (Über das Master-Tag):
1.  Sie müssen die UID eines speziellen "Master-Tags" in der `secrets.yaml` definieren.
2.  Halten Sie das Master-Tag an den Leser. Das System wechselt in den Lernmodus (visuelles/akustisches Feedback).
3.  Halten Sie nun neue Tags an den Leser, um diese anzulernen.

---

## 👆 Fingerabdrücke anlernen

Der R503 Sensor verwaltet bis zu 200 Fingerabdrücke. Die Verwaltung der IDs übernimmt der ESP32 automatisch (`next_finger_id`).

1.  Optional: Namen in das Feld "Name für neuen Zugang" eingeben.
2.  Drücken Sie den Button **"Neuen Finger anlernen (Auto-ID)"**.
3.  Beobachten Sie den Sensor **"Fingerprint Status"**. Er fordert Sie auf, den Finger aufzulegen.
4.  Legen Sie den Finger **zweimal** auf den Sensor (warten Sie auf das Blinken/Piepen dazwischen).
5.  Bei Erfolg meldet der Status "Erfolgreich!" und die ID wird mit dem Namen verknüpft.

---

## 🗑️ Zugänge Löschen

*   Um **alle** Tags zu löschen, drücken Sie in Home Assistant auf **"Whitelist komplett löschen"**.
*   **Einzelne Tags** können derzeit nur durch komplettes Löschen und Neuanlernen der restlichen Tags entfernt werden.
*   **Falsch geschriebene Namen:** Derzeit müssen Sie den entsprechenden Tag/Finger ignorieren oder die Whitelist/Finger löschen und neu anlegen. (Finger können im R503 nicht einzeln überschrieben werden, ohne ein externes Tool zu nutzen).
