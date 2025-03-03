#include <WiFi.h>
#include <PubSubClient.h>
#include <ModbusMaster.h>
#include <WebServer.h>
#include <Preferences.h>
#include "measurements.h"
#include "measurements_config.h"

// Webserver und NVS
WebServer server(80);
Preferences preferences;

// MQTT Client
WiFiClient espClient;
PubSubClient client(espClient);

// Modbus Einstellungen
#define RXD2 27  // UART RX-Pin am ESP32
#define TXD2 26  // UART TX-Pin am ESP32
#define RS485_DIR 18  // DE/RE-Pin (Flussrichtung)

ModbusMaster node;

String ssid;
String password;
String mqtt_server;
String mqtt_user;
String mqtt_password;
String mqtt_port;
String mqtt_topic;

unsigned long loopSecond = 0;
unsigned long lastReconnectAttempt = 0;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Verbinde mit ");
  Serial.println(ssid);

  WiFi.begin(ssid.c_str(), password.c_str());
  
  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi verbunden");
    Serial.print("IP Adresse: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("WiFi Verbindung fehlgeschlagen, wechsle in AP-Modus");
    WiFi.softAP("ESP32_Config_AP");
    Serial.print("AP IP Adresse: ");
    Serial.println(WiFi.softAPIP());
  }
}

boolean reconnect() {
  if (client.connect("ESP32Client", mqtt_user.c_str(), mqtt_password.c_str())) {
    Serial.println("MQTT verbunden");
    return true;
  }
  return false;
}

void handleRoot() {
  Serial.println("Handle root called");
  String html = "<html>\
    <body>\
      <h1>ESP32 Konfiguration</h1>\
      <form action='/save' method='POST'>\
        <label for='ssid'>SSID:</label><br>\
        <input type='text' id='ssid' name='ssid' value='" + ssid + "'><br>\
        <label for='password'>WLAN Passwort:</label><br>\
        <input type='password' id='password' name='password' value='" + password + "'><br>\
        <label for='mqtt_server'>MQTT Server:</label><br>\
        <input type='text' id='mqtt_server' name='mqtt_server' value='" + mqtt_server + "'><br>\
        <label for='mqtt_user'>MQTT Benutzer:</label><br>\
        <input type='text' id='mqtt_user' name='mqtt_user' value='" + mqtt_user + "'><br>\
        <label for='mqtt_password'>MQTT Passwort:</label><br>\
        <input type='password' id='mqtt_password' name='mqtt_password' value='" + mqtt_password + "'><br>\
        <label for='mqtt_topic'>MQTT Topic:</label><br>\
        <input type='text' id='mqtt_topic' name='mqtt_topic' value='" + mqtt_topic + "'><br>\
        <label for='mqtt_port'>MQTT Port:</label><br>\
        <input type='text' id='mqtt_port' name='mqtt_port' value='" + mqtt_port + "'><br><br>\
        <input type='submit' value='Speichern'>\
      </form>\
    </body>\
  </html>";
  server.send(200, "text/html", html);
}

void handleSave() {
  ssid = server.arg("ssid");
  password = server.arg("password");
  mqtt_server = server.arg("mqtt_server");
  mqtt_user = server.arg("mqtt_user");
  mqtt_password = server.arg("mqtt_password");
  mqtt_port = server.arg("mqtt_port");
  mqtt_topic = server.arg("mqtt_topic");

  preferences.putString("ssid", ssid);
  preferences.putString("password", password);
  preferences.putString("mqtt_server", mqtt_server);
  preferences.putString("mqtt_user", mqtt_user);
  preferences.putString("mqtt_password", mqtt_password);
  preferences.putString("mqtt_port", mqtt_port);
  preferences.putString("mqtt_topic", mqtt_topic);

  server.send(200, "text/html", "<html><body><h1>Gespeichert! Neustart erforderlich.</h1></body></html>");
  delay(2000);
  ESP.restart();
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");
}

void handleStatus() {
  String html = "<html>\
    <head><title>ESP32 Statusseite</title>\
    <style>table {width: 100%;border-collapse: collapse;} table, th, td {border: 1px solid black;} th, td {padding: 10px;text-align: left;} th {background-color: #f2f2f2;}</style>\
    </head>\
    <body>\
      <h1>ESP32 Statusseite</h1>\
      <table>\
        <tr><th>Messwert</th><th>Wert</th></tr>";
  for (size_t i = 0; i < numMeasurements; ++i) {
    html += "<tr><td>" + measurements[i].mqtt_topic_suffix + "</td><td>";
    uint16_t result = node.readHoldingRegisters(measurements[i].address, measurements[i].numberOfRegisters);
    if (result == node.ku8MBSuccess) {
      if (measurements[i].isText) {
        char textValue[measurements[i].response_size + 1];
        for (uint8_t reg = 0; reg < measurements[i].numberOfRegisters; ++reg) {
          uint16_t regValue = node.getResponseBuffer(reg);
          textValue[reg * 2] = regValue >> 8;
          textValue[reg * 2 + 1] = regValue & 0xFF;
        }
        textValue[measurements[i].response_size] = '\0';
        html += String(textValue);
      } else {
        uint32_t combinedValue = 0;
        for (uint8_t reg = 0; reg < measurements[i].numberOfRegisters; ++reg) {
          combinedValue = (combinedValue << 16) | node.getResponseBuffer(reg);
        }

        String outputValue;
        auto it = measurements[i].valueMap.find(combinedValue);
        if (it != measurements[i].valueMap.end()) {
          outputValue = it->second;
        } else {
          if (measurements[i].isSigned) {
            if (measurements[i].numberOfRegisters == 1) {
              measurements[i].value = (int16_t)combinedValue * measurements[i].scaleFactor;
            } else if (measurements[i].numberOfRegisters == 2) {
              measurements[i].value = (int32_t)combinedValue * measurements[i].scaleFactor;
            }
          } else {
            measurements[i].value = combinedValue * measurements[i].scaleFactor;
          }
          outputValue = String(measurements[i].value, (int)measurements[i].precision);
        }
        html += outputValue;
      }
    } else {
      html += "Fehler beim Lesen";
    }
    html += "</td></tr>";
  }
  html += "</table></body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  pinMode(RS485_DIR, OUTPUT);
  digitalWrite(RS485_DIR, LOW); // Start im Empfangsmodus

  initModbus(node);

  preferences.begin("my-app", false);
  ssid = preferences.getString("ssid", "");
  password = preferences.getString("password", "");
  mqtt_server = preferences.getString("mqtt_server", "");
  mqtt_user = preferences.getString("mqtt_user", "");
  mqtt_password = preferences.getString("mqtt_password", "");
  mqtt_port = preferences.getString("mqtt_port", "");
  mqtt_topic = preferences.getString("mqtt_topic", "");

  setup_wifi();
  client.setServer(mqtt_server.c_str(), mqtt_port.toInt());

  // Aktualisiere die MQTT-Themen basierend auf dem gespeicherten Basisthema
  updateMQTTTopics(measurements, numMeasurements, mqtt_topic);

  server.on("/", handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/status", handleStatus); // Neue Route für die Statusseite hinzufügen
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Webserver gestartet.");
}

void loop() {
  server.handleClient();

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      unsigned long now = millis();
      if (now - lastReconnectAttempt > 5000) {
        lastReconnectAttempt = now;
        if (reconnect()) {
          lastReconnectAttempt = 0;
        }
      }
    } else {
      client.loop();
      readAndPublishMeasurements(node, client, measurements, numMeasurements);
    }
  }

  delay(2000);  // 2 Sekunden warten
}