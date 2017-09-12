/*
      Project name: Apple Homebridge – MQTT Temperature-Humidity Sensor
      Project URI: https://www.studiopieters.nl/apple-homebridge-mqtt-temperature-humidity-sensor
      Description: Apple Homebridge read DHT11 Over MQTT
      Version: 0.1.6   License: MIT
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);


const char *ssid =	"xxxxxxxx";		// cannot be longer than 32 characters!
const char *pass =	"yyyyyyyy";		//

// Update these with values suitable for your network.
IPAddress server(172, 16, 0, 2);

void callback(const MQTT::Publish& pub) {
  // handle message arrived
}

WiFiClient wclient;
PubSubClient client(wclient, server);

void setup() {
  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  dht.begin();
  client.set_callback(callback);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      if (client.connect("arduinoClient")) {

      }
    }

    if (client.connected())
      client.loop();
    delay(2000);
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");

    client.publish("/sensors/Temperaturesensor", String (t));
    client.publish("/sensors/Humiditysensor", String (h));
    client.subscribe(MQTT::Subscribe()
                     .add_topic("/sensors/Temperaturesensor")
                     .add_topic("/sensors/Humiditysensor")
                    );
  }
}

