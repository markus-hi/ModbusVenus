#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include <ModbusMaster.h>
#include <PubSubClient.h>
#include <map>

// Struktur zur Speicherung der Modbus-Adressen und zugehörigen Infos
struct Measurement {
    uint16_t address;
    uint8_t numberOfRegisters;
    String mqtt_topic_suffix;
    float scaleFactor;
    unsigned long scan_interval;
    unsigned long last_scan_time;
    float value;
    String mqtt_topic;
    bool isSigned;
    uint8_t precision;
    bool isText;
    uint8_t response_size;
    std::map<uint16_t, String> valueMap; // Map für individuelle Textwerte
};

// Funktion zur Initialisierung der Modbus-Kommunikation
void initModbus(ModbusMaster& node);

// Funktion zur Messwerterfassung und Veröffentlichung über MQTT
void readAndPublishMeasurements(ModbusMaster& node, PubSubClient& client, Measurement* measurements, size_t numMeasurements);

// Funktion zur Aktualisierung der MQTT-Themen basierend auf dem Basisthema
void updateMQTTTopics(Measurement* measurements, size_t numMeasurements, const String& base_topic);

#endif // MEASUREMENTS_H