#include "measurements.h"
#include <Arduino.h>

void initModbus(ModbusMaster& node) {
    // Modbus-Einstellungen
    node.begin(1, Serial2); // Modbus-Adresse 1
    node.preTransmission([]() {
        digitalWrite(18, HIGH); // Senden aktivieren
        delayMicroseconds(10);  // Kurze Wartezeit
    });
    node.postTransmission([]() {
        delayMicroseconds(100);  // Wartezeit für sicheres Ende des Sendevorgangs
        digitalWrite(18, LOW);  // Empfangen aktivieren
    });
}

void readAndPublishMeasurements(ModbusMaster& node, PubSubClient& client, Measurement* measurements, size_t numMeasurements) {
    for (size_t i = 0; i < numMeasurements; ++i) {
        unsigned long current_time = millis();
        if (current_time - measurements[i].last_scan_time >= measurements[i].scan_interval) {
            measurements[i].last_scan_time = current_time;
            uint16_t result = node.readHoldingRegisters(measurements[i].address, measurements[i].numberOfRegisters);
            if (result == node.ku8MBSuccess) {
                if (measurements[i].isText) {
                    // Textwert auslesen
                    char textValue[measurements[i].response_size + 1];
                    for (uint8_t reg = 0; reg < measurements[i].numberOfRegisters; ++reg) {
                        uint16_t regValue = node.getResponseBuffer(reg);
                        textValue[reg * 2] = regValue >> 8;
                        textValue[reg * 2 + 1] = regValue & 0xFF;
                    }
                    textValue[measurements[i].response_size] = '\0';
                    String textString = String(textValue);
                    Serial.print(measurements[i].mqtt_topic);
                    Serial.print(": ");
                    Serial.println(textString);
                    client.publish(measurements[i].mqtt_topic.c_str(), textString.c_str());
                } else {
                    // Zahlenwert auslesen
                    uint32_t combinedValue = 0;
                    for (uint8_t reg = 0; reg < measurements[i].numberOfRegisters; ++reg) {
                        combinedValue = (combinedValue << 16) | node.getResponseBuffer(reg);
                    }

                    if (measurements[i].isSigned) {
                        if (measurements[i].numberOfRegisters == 1) {
                            measurements[i].value = (int16_t)combinedValue * measurements[i].scaleFactor;
                        } else if (measurements[i].numberOfRegisters == 2) {
                            measurements[i].value = (int32_t)combinedValue * measurements[i].scaleFactor;
                        }
                    } else {
                        measurements[i].value = combinedValue * measurements[i].scaleFactor;
                    }

                    // Überprüfen, ob ein individueller Textwert vorhanden ist
                    String outputValue;
                    auto it = measurements[i].valueMap.find(combinedValue);
                    if (it != measurements[i].valueMap.end()) {
                        outputValue = it->second;
                    } else {
                        outputValue = String(measurements[i].value, (int)measurements[i].precision);
                    }

                    Serial.print(measurements[i].mqtt_topic);
                    Serial.print(": ");
                    Serial.println(outputValue);
                    client.publish(measurements[i].mqtt_topic.c_str(), outputValue.c_str());
                }
            } else {
                Serial.print("Fehler beim Lesen der Adresse: ");
                Serial.println(measurements[i].address, HEX);
            }
        }
    }
}

void updateMQTTTopics(Measurement* measurements, size_t numMeasurements, const String& base_topic) {
    for (size_t i = 0; i < numMeasurements; ++i) {
        measurements[i].mqtt_topic = base_topic + "/" + measurements[i].mqtt_topic_suffix;
    }
}