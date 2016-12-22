Channels
========

Wie sollten Channels gewählt werden?

Der erste Teil spezifiziert die Domain:
> /#domain#/

In unserem Fall immer 
> /oadrive/

Dann kommt die Verbindung (der "Gesprächspartner", immer vom server aus):

> /oadrive/#destination#/

Verbindungen sind entweder goffin, inka oder web (der client der im Browser läuft).

> /oadrive/goffin/

Dann gibt man den Typ der Nachricht an:

> /oadrive/#destination#/#typ#/

Types können z.B. info oder commands sein

> /oadrive/goffin/commands/

Und zum Schluss noch den Name worum es sich handelt

> /oadrive/#destination#/#typ#/#name#


Genutze Channels
-----------------

Bitte listet hier alle Channels auf die ihr in irgendeiner From nutzt. Gerne auch mit Kommentar.

> oadrive/goffin/commands
> oadrive/inka/commands
Um den Autos Befehle zu übermitteln

Um inka Befehle zu übermitteln

Werte die geschickt werden können sind:
  - "left"
  - "straight"
  - "right"
  - "turn"
  - "speed_boost"
  - "speed_normal"
  - "game_finished"

=======
> oadrive/goffin/info/status
> oadrive/inka/info/status
Status-updates von den Autos an den server
Werte die geschickt werden sind:
  - "connected"
  - "reached_crossing"
  - "entered_crossing"
  - "finished_crossing"
  - "disconnected"
  - "game_finished"

> oadrive/goffin/info/distance
> oadrive/inka/info/distance
Distance zu Marker von den Autos an den server

Tipps und Tricks
=================

Um sich Testdaten aufzunehmen kann man einfach folgenden Befehl auf dem Server (wo redis läuft) ausführen.

> redis-cli subscribe channel >> log.txt

also z.B.

> redis-cli subscribe oadrive/goffin/info/map >> maplog.txt

Bedienung
==================
Auto rausgefahren
- Einfach wieder auf Fahrbahn stellen und kurz warten

Spiel stoppen
- Stop über Admin-Interface

Spiel (neu) starten
- Spiel starten über Admin-Interface
- Autos übers Jury Modul starten

Teams wechseln
- Environment Reset über Admin-Interface
- Teams switchen über Admin-Interface
- Autos über Jury Modul starten
