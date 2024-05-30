#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include "DHTesp.h"
#include <PubSubClient.h>

#define DHTPIN 15
#define BUZZER 18
DHTesp dht;


#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASWORD ""
#define MQTT_SERVER "test.mosquitto.org"
#define MQTT_TOPIC "sensor/data"

WiFiClient espClient;
PubSubClient mqtt(espClient);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void connect_wifi()
{
  lcd.setCursor(0, 0);
  lcd.print("Connecting to Wifi");

  WiFi.begin(WIFI_SSID, WIFI_PASWORD);
  lcd.setCursor(0, 1);
  while (WiFi.status() != WL_CONNECTED ) {
    delay(500);
    lcd.print(".");
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected to Wifi");
}

void connect_mqtt() {
  // Loop until we're reconnected
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqtt.connect(clientId.c_str())) {
      Serial.println("Connected");
      // Once connected, publish an announcement
      mqtt.publish(MQTT_TOPIC, "Connected");
    } else {
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  dht.setup(DHTPIN, DHTesp::DHT22);// initialize the sensor
  pinMode(BUZZER, OUTPUT);
  pinMode(5, OUTPUT);
  lcd.init();
  lcd.backlight();

  mqtt.setServer(MQTT_SERVER, 1883);

  connect_wifi();
  delay(1000);
  lcd.clear();
}

void clearLCDLine(int line)
{
  lcd.setCursor(0, line);
  for (int i = 0; i < 20; i++) {
    lcd.print(" ");
  }
  lcd.setCursor(0, line);
}

void loop() {
  char temp[10] = {0};
  char humid[10] = {0};
  String status;


  connect_mqtt();

  sprintf(humid, "%.2f", dht.getHumidity());
  lcd.setCursor(0, 0); // set the cursor on the first row and column
  lcd.print("Humid=");
  lcd.print(humid);//print the humidity
  lcd.print("%");

  sprintf(temp, "%.2f", dht.getTemperature());
  lcd.setCursor(0, 1); //set the cursor on the second row and first column
  lcd.print("Temp=");
  lcd.print(temp);//print the temperature

  float h = dht.getHumidity();
  float t = dht.getTemperature();

  clearLCDLine(2);

  if (h < 30 && t > 30) {
    status = "High Risk!";
    digitalWrite(5, HIGH);
    tone(BUZZER, 165);
  }

  else if (h > 30 && h < 50 && t > 30) {
    status = "Moderate Risk";
    digitalWrite(5, LOW);
    noTone(BUZZER);
  }

  else if (h >= 50 && t > 20) {
    status = "Low Risk";
    digitalWrite(5, LOW);
    noTone(BUZZER);
  }

  else {
    status = "Safe";
    digitalWrite(5, LOW);
    noTone(BUZZER);
  }

  lcd.print(status);

  DynamicJsonDocument doc(1024);
  doc["status"] = status;
  doc["temp"] = t;
  doc["humid"] = h;

  String payload;
  serializeJson(doc, payload);
  mqtt.publish(MQTT_TOPIC, payload.c_str());
  delay(30000);

}