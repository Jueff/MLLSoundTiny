# MLLSoundTiny (English)

(deutsche Version weiter unten)

MLLSoundTiny is an Arduino library for controlling sound modules, particularly for integration with LED data in model railway applications. It enables processing of LED signals, mapping them to sound commands, and managing multiple sound modules via serial interfaces.

## Features

- **LED Data Processing**: Processes incoming LED data and maps it to sound commands.
- **Multiple Sound Modules**: Supports multiple modules simultaneously with queue management.
- **Module Type Support**: Compatible with MP3-TF-16P and JQ6500 modules.
- **Flash Storage**: Stores module types persistently in flash memory.
- **Command Mapping**: Handles different command formats for various modules.
- **Repeat Modes**: Supports playing tracks with repeat.

## Class Overview

### LedToSound

This class processes incoming LED data, maps it to sound commands, and manages a queue to send these commands to multiple sound modules. It supports setting active modules, playing tracks, and handling repeat modes.

### MultiPlayer

This class provides an interface to send commands to different sound modules, such as MP3-TF-16P and JQ6500, over a serial stream. It handles command mapping, checksum calculation, and module type switching.

### SoundModule

This class represents a sound module connected via serial, handles initialization, stores the module type in flash memory, and processes commands by forwarding them to the MultiPlayer instance.

## License

This library is released under the LGPL-2.1-or-later license. See the SPDX license headers in the source files for details.


---

# MLLSoundTiny (Deutsch)

MLLSoundTiny ist eine Arduino-Bibliothek zur Steuerung von Sound-Modulen, insbesondere für die Integration mit LED-Daten in Modellbahn-Anwendungen. Sie ermöglicht die Verarbeitung von LED-Signalen, die Zuordnung zu Sound-Befehlen und die Verwaltung mehrerer Sound-Module über serielle Schnittstellen.

## Funktionen

- **LED-Daten-Verarbeitung**: Verarbeitet eingehende LED-Daten und mappt sie auf Sound-Befehle.
- **Mehrere Sound-Module**: Unterstützt mehrere Module gleichzeitig mit Warteschlangen-Verwaltung.
- **Modul-Typ-Unterstützung**: Kompatibel mit MP3-TF-16P und JQ6500 Modulen.
- **Flash-Speicher**: Speichert Modul-Typen persistent im Flash-Speicher.
- **Befehls-Mapping**: Handhabt unterschiedliche Befehlsformate für verschiedene Module.
- **Wiederholungsmodi**: Unterstützt das Abspielen von Tracks mit Wiederholung.

## Klassen-Übersicht

### LedToSound

Diese Klasse verarbeitet eingehende LED-Daten, mappt sie auf Sound-Befehle und verwaltet eine Warteschlange, um diese Befehle an mehrere Sound-Module zu senden. Sie unterstützt das Setzen aktiver Module, das Abspielen von Tracks und die Handhabung von Wiederholungsmodi.

### MultiPlayer

Diese Klasse bietet eine Schnittstelle zum Senden von Befehlen an verschiedene Sound-Module, wie MP3-TF-16P und JQ6500, über einen seriellen Stream. Sie handhabt Befehls-Mapping, Prüfsummenberechnung und Modul-Typ-Wechsel.

### SoundModule

Diese Klasse repräsentiert ein Sound-Modul, das über Serial verbunden ist, handhabt die Initialisierung, speichert den Modul-Typ im Flash-Speicher und verarbeitet Befehle, indem sie diese an die MultiPlayer-Instanz weiterleitet.
