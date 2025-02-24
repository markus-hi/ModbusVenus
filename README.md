# ModbusVenus

Dieses Repository ermöglicht das Auslesen von Messdaten aus einem PV-Speicher des Modells Venus E von Marstek. Die Kommunikation erfolgt über das Modbus-Protokoll.

## Inhaltsverzeichnis
- [Installation](#installation)
- [Verwendung](#verwendung)
- [Konfiguration](#konfiguration)
- [Beispiel](#beispiel)
- [Unterstützung und Beiträge](#unterstützung-und-beiträge)
- [Lizenz](#lizenz)

## Installation
1. Klone dieses Repository:
    ```sh
    git clone https://github.com/markus-hi/ModbusVenus.git
    ```
2. Wechsle in das Verzeichnis:
    ```sh
    cd ModbusVenus
    ```
3. Installiere die benötigten Abhängigkeiten:
    ```sh
    pip install -r requirements.txt
    ```

## Verwendung
Um die Messdaten auszulesen, führe das folgende Skript aus:
```sh
python read_data.py
