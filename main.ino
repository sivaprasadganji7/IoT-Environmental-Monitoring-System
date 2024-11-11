#include <WiFi.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define LED_R_PIN 16
#define LED_G_PIN 17
#define LED_B_PIN 18
#define DHT_PIN 4
#define DHTTYPE DHT11
#define TEMP_ALARM_LOW 0.0
#define TEMP_WARN_LOW 10.0
#define TEMP_WARN_HIGH 25.0
#define TEMP_ALARM_HIGH 30.0

const char* ssid = "SIVAPRASAD";
const char* pass = "rama1234";
#define MQTT_HOST "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "esp32-2793FC"
#define MQTT_TOPIC_TEMP "esp32/temperature_sensor/evt/status"
#define MQTT_TOPIC_HUMID "esp32/humidity_sensor/evt/status"
#define MQTT_TOPIC_LED_CONTROL "esp32/led_control"
#define MQTT_TOPIC_INTERVAL "esp32/set_interval"

bool manualModeActive = false;
unsigned long manualModeStartTime;
const unsigned long manualModeDuration = 5000;
DHT dht(DHT_PIN, DHTTYPE);
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);
unsigned long lastPublishTime = 0;
unsigned long publishInterval = 10000;
float humidity = 0.0;
float temperature = 0.0;

bool connectToMQTT();

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    Serial.printf("Received message on topic: %s\n", topic);
    String command = "";
    for (int i = 0; i < length; i++) command += (char)payload[i];
    if (String(topic) == MQTT_TOPIC_INTERVAL) {
        DynamicJsonDocument doc(200);
        if (deserializeJson(doc, command) == DeserializationError::Ok && doc.containsKey("Interval")) {
            publishInterval = doc["Interval"].as<unsigned long>() * 1000;
            Serial.printf("Updated interval: %lu seconds\n", publishInterval / 1000);
        }
    } else {
        manualModeActive = true;
        manualModeStartTime = millis();
        if (command == "red") setColor(0, 255, 255);
        else if (command == "green") setColor(255, 0, 255);
        else if (command == "blue") setColor(255, 255, 0);
        else if (command == "yellow") setColor(0, 0, 255);
        else if (command == "off") setColor(255, 255, 255);
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting ESP32 Sensor Application");
    pinMode(LED_R_PIN, OUTPUT);
    pinMode(LED_G_PIN, OUTPUT);
    pinMode(LED_B_PIN, OUTPUT);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");
    dht.begin();
    mqtt.setServer(MQTT_HOST, MQTT_PORT);
    mqtt.setCallback(mqttCallback);
    if (connectToMQTT()) {
        mqtt.subscribe(MQTT_TOPIC_LED_CONTROL);
        mqtt.subscribe(MQTT_TOPIC_INTERVAL);
    }
}

bool connectToMQTT() {
    while (!mqtt.connected()) {
        Serial.print("Connecting to MQTT...");
        if (mqtt.connect(MQTT_CLIENT_ID)) {
            Serial.println("MQTT connected");
            mqtt.subscribe(MQTT_TOPIC_LED_CONTROL);
            mqtt.subscribe(MQTT_TOPIC_INTERVAL);
            return true;
        } else {
            Serial.print("MQTT connection failed, retrying...");
            delay(5000);
        }
    }
    return false;
}

void setColor(uint8_t r, uint8_t g, uint8_t b) {
    digitalWrite(LED_R_PIN, r ? HIGH : LOW);
    digitalWrite(LED_G_PIN, g ? HIGH : LOW);
    digitalWrite(LED_B_PIN, b ? HIGH : LOW);
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Reconnecting to WiFi...");
        WiFi.begin(ssid, pass);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("WiFi reconnected");
    }
    if (!mqtt.connected()) {
        if (connectToMQTT()) Serial.println("MQTT reconnected");
        else {
            delay(5000);
            return;
        }
    }
    mqtt.loop();
    if (manualModeActive && millis() - manualModeStartTime > manualModeDuration) manualModeActive = false;
    if (!manualModeActive && millis() - lastPublishTime >= publishInterval) {
        lastPublishTime = millis();
        humidity = dht.readHumidity();
        temperature = dht.readTemperature();
        if (isnan(humidity) || isnan(temperature)) Serial.println("Failed to read from DHT sensor");
        else {
            char tempMsg[50];
            char humMsg[50];
            snprintf(tempMsg, sizeof(tempMsg), "%.2f", temperature);
            snprintf(humMsg, sizeof(humMsg), "%.2f", humidity);
            mqtt.publish(MQTT_TOPIC_TEMP, tempMsg);
            mqtt.publish(MQTT_TOPIC_HUMID, humMsg);
            if (temperature < TEMP_ALARM_LOW) setColor(0, 0, 255);
            else if (temperature < TEMP_WARN_LOW) setColor(0, 255, 255);
            else if (temperature <= TEMP_WARN_HIGH) setColor(0, 255, 0);
            else if (temperature < TEMP_ALARM_HIGH) setColor(255, 255, 0);
            else setColor(255, 0, 0);
        }
    }
}
