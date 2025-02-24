# ModbusVenus

Dieses Repository ermöglicht das Auslesen von Messdaten aus einem PV-Speicher des Modells Venus E von Marstek. Die Kommunikation erfolgt über das Modbus-Protokoll. Ich habe das unter Visual Studio Code mit dem Platformio Plugin entwickelt.

## Inhaltsverzeichnis
- [Installation](#installation)
- [Verwendung](#verwendung)
- [Konfiguration](#konfiguration)
- [Hinweis](#hinweis)


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
4. Flashen über die Funktion von VSC. Nach diesem Vorgang sind auf der Konsole Ausgaben zu sehen.

## Verwendung
Zur Erstkonfiguration wechselt der ESP32 in den AP-Modus und ist als WLAN "EPS32_Config_AP" zu finden.

## Konfiguration
Sobald man sich damit verbunden hat, ist unter der IP "192.168.4.1" eine einfache Webkonfiguration möglich. Dort findet man neben den WLAN Einstellungen auch die MQTT-Broker Daten zum eingeben. Nach dem Speichern startet der µC neu und verbindet sich mit dem WLAN sowie dem MQTT-Broker. Der Status dieser Vorgänge ist jeweils auf der seriellen Konsole zu sehen.

## Hinweis
Es kann sein, dass nach dem ersten Start keine Daten geliefert werden. Das kann an einer fehlerhaften Modbus Implementierung seitens Marstek liegen, in dem Fall schlägt die CRC-Prüfung fehl. Um das Problem zu umgehen, kann man in der  ModbusMaster.cpp die CRC-Prüfung deaktivieren:

```c++
    if (!u8MBStatus && (lowByte(u16CRC) != u8ModbusADU[u8ModbusADUSize - 2] ||
      highByte(u16CRC) != u8ModbusADU[u8ModbusADUSize - 1]))
    {
      //u8MBStatus = ku8MBInvalidCRC;
    }
```
